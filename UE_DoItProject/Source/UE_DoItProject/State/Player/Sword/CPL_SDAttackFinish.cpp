#include "CPL_SDAttackFinish.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_BlendCameraComp.h"

UCPL_SDAttackFinish::UCPL_SDAttackFinish()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(ESD_FinalAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(ESD_FinalAttack::END);

		AttackRange = 130.0f;
	}

	FString Path = L"";
	#pragma region Sword FinalAttack Montages
	// Sword FinalAttack Montages
	{
		// @1
		UAnimMontage* Sword_FinalAttack_1 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_FinishAttack_Final.SwordMon_FinishAttack_Final'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFinalAttack_1(*Path);
		if (MFinalAttack_1.Succeeded())
			Sword_FinalAttack_1 = MFinalAttack_1.Object;

		SwordAttackMontages.Emplace(Sword_FinalAttack_1);
	}

	#pragma endregion

	#pragma region DamageType

	DT_Noraml		= NewObject<UCDamageType_Normal>();
	DT_Noraml->SetDamageImpulse(10.0f);

	DT_StrongAttack = NewObject<UCDamageType_StrongAttack>();
	DT_StrongAttack->SetDamageImpulse(20.0f);

	#pragma endregion
}

void UCPL_SDAttackFinish::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackFinish::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCPL_SDAttackFinish::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion

	PlayerController = Cast<APlayerController>(Player->GetController());
	check(PlayerController);

#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		//@ON AI - Target
		if (Target != nullptr)
		{
			IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
			if (I_Monster != nullptr)
			{
				I_Monster->SetAIRunningPossible(true);
			}

			Target = nullptr;
		}
	});

	//BeginAttackDeleFunc.AddUObject(this, &UCPL_SDAttackFinish::DelegateBeginAttack);
	EndAttackDeleFunc.AddUObject(this, &UCPL_SDAttackFinish::EndAttack);

#pragma endregion
}

// - IBaseAttack ����.
// @param DoingActor - �����ϴ� ��ü
// #Edit *0310
// @Warning - 
// ��Ÿ�ָ� ������ ���� ����. �׷��� Player �� Input ���� ���
// ���������� �����ϱ� ���ؼ� �׻�, ComboCheck = true
void UCPL_SDAttackFinish::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick ���� ó�� ��.
	}

	//@IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling() && Player->GetCharacterMovement()->GravityScale > 0.0f); //@InAir
	IfTrueRet(IsLastCombo()); //@IsLastCombo

	Target = Player->GetFindAttackTarget();
	if (Target == nullptr)
	{
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	// @Target �� ���� �ִٸ� ���� ��� �ߴ�.
	ACharacter* TargetCharactor = Cast<ACharacter>(Target);
	if (TargetCharactor != nullptr)
	{
		if (UCFL_ActorAgainst::IsTargetInAir(TargetCharactor) == true)
		{
			AttackDecision->StopAttackTrace();
			return;
		}
	}

	//@TODO : Target �� ������ �ϰ��� ���� ü���� �ƴ϶��,
	//@0623 �켱 ī�޶� RelativePosition �ٽ� ��������....
	// �ȳ��� ����.
	// ���� ��� �����鼭 ���� ���� ��Ȳ���� ���,
	// ��(Sword) �� ȭ��, ��, ���̽� �нú� ��ų..
	// ȭ���� ������ ����, ���� ���� �ɸ�, ���̽��� ����Ȯ���� ����.


////////////////////////////////////////////////////////////////////////////////////////////////////

	//@ON BlockInput
	Player->OnBlockKeyInput();

	//@OFF AI - Target
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
	if (I_Monster != nullptr)
	{
		I_Monster->SetAIRunningPossible(false);
	}

	//@Ÿ�� �ٶ󺸰� �ϱ�
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	//@�Ÿ� ������, ���� ���߱�
	UCFL_ActorAgainst::ActorLocateFrontTarget(Target, Player, AttackRange, true);

	//@���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 0.7f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.1f, true				// @AnimPlay ������ ����.
		);
	}

	// @ComboCheck == true �� ��
	bComboCheck = true;
}

void UCPL_SDAttackFinish::EndAttack()
{
	Super::EndAttack();

	//@OFF BlockInput
	Player->OffBlockKeyInput();

	//@ON AI - Target
	if (Target != nullptr)
	{
		IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
		if (I_Monster != nullptr)
		{
			I_Monster->SetAIRunningPossible(true);
		}

		Target = nullptr;
	}
}

// @Combo �� ������ ������ ��Ȯ�� �˱� ���ؼ�.
bool UCPL_SDAttackFinish::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(ESD_FinalAttack::END))
		return true;

	return false;
}

/* �ٸ� Pawn �� ���� ó�� �Լ� */
// @DoingActor - Attack �� �� ��ü ��, ���⼱ Player (Owner)
void UCPL_SDAttackFinish::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel
	(
		HitResults
		, Start
		, End
		, FQuat::Identity
		, ECC_GameTraceChannel2 // @PlayerAttack
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	float DebugLifeTime = 1.0f;
	//DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		for (FHitResult& HitResult : HitResults)
		{
			IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
			if (Charactor != nullptr)
			{
				// 1. Get Interface HitComp
				IIC_HitComp* HitComp = Charactor->GetIHitComp();
				if (HitComp != nullptr)
				{
					if (HitResult.GetActor() == Target)
					{
						++CurrentComboNum; //@�޺� �÷��ֱ�
					}
					
					if (IsLastCombo() == true)
					{
						//@������ �ϰ� - StrongAttack
						FVector HitDirection = Player->GetActorForwardVector();
						HitDirection.Z = 0.0f;
						HitComp->SetHitDirection(HitDirection);
						HitComp->SetHitMoveSpeed(0.3f);
						HitComp->OnHit(Player, DT_StrongAttack, 0.0f);
					}
					else
					{
						//@NormalAttack - Speed(0.0f)
						FVector HitDirection = Player->GetActorForwardVector();
						HitDirection.Z = 0.0f;
						HitComp->SetHitDirection(HitDirection);
						HitComp->SetHitMoveSpeed(0.0f);
						HitComp->OnHit(Player, DT_Noraml, 0.0f);
					}

				}//(HitComp != nullptr)
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
			}//(Charactor != nullptr)
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");

		}//for(HitResult)
	}//(bHit == true)
}
