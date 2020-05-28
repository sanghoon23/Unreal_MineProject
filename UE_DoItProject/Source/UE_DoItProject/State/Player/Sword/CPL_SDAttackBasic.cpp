#include "CPL_SDAttackBasic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "DamageType/CDamageType_Normal.h"

UCPL_SDAttackBasic::UCPL_SDAttackBasic()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Super
	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(USD_BasicAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(USD_BasicAttack::END);
	}
	#pragma endregion

	FString Path = L"";

	#pragma region SwordAttackMontage
	// SwordAttackMontage
	{
		UAnimMontage* Sword_BasicAttack_1 = nullptr;
		UAnimMontage* Sword_BasicAttack_2 = nullptr;
		UAnimMontage* Sword_BasicAttack_3 = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/Sword_BasicAttack1.Sword_BasicAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_1(*Path);
		if (MBasicAttack_1.Succeeded())
			Sword_BasicAttack_1 = MBasicAttack_1.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/Sword_BasicAttack2.Sword_BasicAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_2(*Path);
		if (MBasicAttack_2.Succeeded())
			Sword_BasicAttack_2 = MBasicAttack_2.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/Sword_BasicAttack3.Sword_BasicAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_3(*Path);
		if (MBasicAttack_3.Succeeded())
			Sword_BasicAttack_3 = MBasicAttack_3.Object;

		SwordAttackMontages.Emplace(Sword_BasicAttack_1);
		SwordAttackMontages.Emplace(Sword_BasicAttack_2);
		SwordAttackMontages.Emplace(Sword_BasicAttack_3);
	}
	#pragma endregion

	#pragma region Create DamageType

	DT_Noraml = NewObject<UCDamageType_Normal>();

	#pragma endregion
}

void UCPL_SDAttackBasic::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//Test Code
	AttackDecision->UnAble();

	//@Before
	//@Auto AttackDecision System
	//AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion
}

void UCPL_SDAttackBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UCPL_SDAttackBasic::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

// - IBaseAttack ����.
// @param DoingActor - �����ϴ� ��ü
// @Warning - ���� Anim �� ������ true �� ����
// ���������� InputKey �� ���ö�,
// ������ ��Ÿ�ְ� ������ ��Ÿ���� bAttacking == false �� ����.(ComboNotify �� EndAttack ��)
void UCPL_SDAttackBasic::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick ���� ó�� ��.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	if (Target != nullptr)
	{
		//@ Ÿ�� �ٶ󺸰� �ϱ�
		UCFL_ActorAgainst::LookAtTarget(Player, Target);
	}

	//@Before
	//APawn* Target = Player->GetFindAttackTarget();
	//if (Target == nullptr)
	//{
	//	EndAttackDeleFunc.Broadcast();
	//	return;
	//}
	//check(Target);
	//// @Ÿ�� �ٶ󺸰� �ϱ�
	//UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay ������ ����.
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

// - IBaseAttack ����.
void UCPL_SDAttackBasic::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	APawn* Target = Player->GetFindAttackTarget();
	if (Target != nullptr)
	{
		//@ Ÿ�� �ٶ󺸰� �ϱ�
		UCFL_ActorAgainst::LookAtTarget(Player, Target);
	}

	//@Before
	//if (Target == nullptr)
	//{
	//	EndAttackDeleFunc.Broadcast();
	//	Player->ActorStopAnimMon(SwordAttackMontages[CurrentComboNum]);
	//	return;
	//}
	//check(Target);

	//	//@ Ÿ�� �ٶ󺸰� �ϱ�
	//UCFL_ActorAgainst::LookAtTarget(Player, Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bComboCheck = false;
	++CurrentComboNum;

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	// ���� �˻�.( CurrentComboNum = 0 ~ MaxCombo ���� )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Charactor->ActorAnimMonPlay
		(
			SwordAttackMontages[CurrentComboNum],
			1.0f, false
		);
	}
	else
	{
		EndAttack();
	}
}

// @Combo �� ������ ������ ��Ȯ�� �˱� ���ؼ�.
bool UCPL_SDAttackBasic::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(USD_BasicAttack::COMBO_THREE))
		return true;

	return false;
}

/* �ٸ� Pawn �� ���� ó�� �Լ� */
void UCPL_SDAttackBasic::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	FHitResult HitResult;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepSingleByChannel //@Single - ����.
	(
		HitResult
		, Start
		, End
		, FQuat::Identity
		, ECC_GameTraceChannel2 // @PlayerAttack
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
		if (Charactor != nullptr)
		{
			// 1. Get Interface HitComp
			IIC_HitComp* HitComp = Charactor->GetIHitComp();
			if (HitComp != nullptr)
			{
				// 1.1 Set Hit Attribute
				FVector HitDirection = Player->GetActorForwardVector();
				HitDirection.Z = 0.0f;
				HitDirection.Normalize();
				HitComp->SetHitDirection(HitDirection);
				HitComp->SetHitMoveSpeed(0.3f);

				// 1.2 Hit Delegate - Normal(DamageType)
				HitComp->OnHit(Player, DT_Noraml, 5.0f);
			}
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
		}
		else
			UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
	}
}

void UCPL_SDAttackBasic::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	TArray<FHitResult> HitResults;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepMultiByChannel //@��ݷ� �ֱ� - Multi
	(
		HitResults
		, Start
		, End
		, FQuat::Identity
		, ECC_GameTraceChannel2 // @PlayerAttack
		, sphere
		, CollisionQueryParm
	);

	if (bHit == true)
	{
		for (auto& Hit : HitResults)
		{
			if (Hit.GetActor()->IsA<APawn>()) // Pawn �̸�, Continue;
				continue;

			if (Cast<ADestructibleActor>(Hit.GetActor()))
			{
				UDestructibleComponent* destructible
					= Cast<UDestructibleComponent>(Hit.GetActor()->GetComponentByClass(UDestructibleComponent::StaticClass()));
				if (destructible != nullptr)
				{
					destructible->ApplyDamage(5.0f, Hit.GetActor()->GetActorLocation(), ActorForward, 1.0f);

					// Test Code
					CLog::Print(Hit.GetActor()->GetName());
				}
				continue;
			}

			CLog::Print(L"SDAttackBasic _ DamageEvnet In!!");

			FDamageEvent DamageEvent;
			Hit.GetActor()->TakeDamage(1.0f, DamageEvent, Player->GetController(), Hit.GetActor());
		}
	}//(bHit)
}

void UCPL_SDAttackBasic::CheckProcedural()
{
	Super::CheckProcedural();
}
