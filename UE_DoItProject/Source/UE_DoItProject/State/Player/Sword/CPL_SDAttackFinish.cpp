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

#include "UI/HUD_Main.h"
#include "UI/Widget/WG_FloatingCombo.h"

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
	AttackDecision->OnAble(Player, AttackRange + 200.0f);

	#pragma endregion


#pragma region UI
	//@UI
	check(MainHUD);
	TargetInfoWidget = MainHUD->GetWidgetTargetInfo();

#pragma endregion

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

// - IBaseAttack 참고.
// @param DoingActor - 공격하는 주체
// #Edit *0310
// @Warning - 
// 몽타주를 나눠서 쓰고 있음. 그래서 Player 의 Input 값이 없어도
// 연속적으로 동작하기 위해서 항상, ComboCheck = true
void UCPL_SDAttackFinish::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick 에서 처리 중.
	}

	//@IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling() && Player->GetCharacterMovement()->GravityScale > 0.0f); //@InAir
	IfTrueRet(IsLastCombo()); //@IsLastCombo

	Target = Player->GetFindAttackTarget();
	if (Target == nullptr)
	{
		//@
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	//@UI
	check(TargetInfoWidget);
	if (TargetInfoWidget->GetInfoMonsterPercentHP() > 0.3f) //@30퍼 이상이면,
	{
		FString Input = L"공격 조건이 충족하지 않습니다!!";
		MainHUD->VisibleUITextNotify(Input, 3.0f);
		return;
	}
	//else
	//{
	//	//@마무리 일격을 위한 데미지 세팅
	//	//DT_StrongAttack->SetDamageImpulse(TargetInfoWidget->GetInfoMonsterCurrentHP() + 0.1f);
	//}

	// @Target 이 공중 있다면 공격 명령 중단.
	ACharacter* TargetCharactor = Cast<ACharacter>(Target);
	if (TargetCharactor != nullptr)
	{
		if (UCFL_ActorAgainst::IsTargetInAir(TargetCharactor) == true)
		{
			AttackDecision->StopAttackTrace();
			return;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	//@ON BlockInput
	Player->OnBlockKeyInput();

	//@OFF AI - Target
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
	if (I_Monster != nullptr)
	{
		I_Monster->SetAIRunningPossible(false);
	}

	//@타겟 바라보게 하기
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	//@거리 벌리고, 높이 맞추기
	UCFL_ActorAgainst::ActorLocateFrontTarget(Target, Player, AttackRange, true);

	//@공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 0.7f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.1f, true				// @AnimPlay 무조건 실행.
		);
	}

	// @ComboCheck == true 로 함
	bComboCheck = true;
}

void UCPL_SDAttackFinish::EndAttack()
{
	Super::EndAttack();

	//@OFF BlockInput
	Player->OffBlockKeyInput();

	////@ON AI - Target
	//if (Target != nullptr)
	//{
	//	IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
	//	if (I_Monster != nullptr)
	//	{
	//		I_Monster->SetAIRunningPossible(true);
	//	}

	//	Target = nullptr;
	//}
}

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCPL_SDAttackFinish::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(ESD_FinalAttack::END))
		return true;

	return false;
}

/* 다른 Pawn 을 공격 처리 함수 */
// @DoingActor - Attack 을 할 객체 즉, 여기선 Player (Owner)
void UCPL_SDAttackFinish::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

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
						++CurrentComboNum; //@콤보 늘려주기
					}
					
					// 1.1 Set Hit Attribute
					FVector HitDirection = Player->GetActorForwardVector();
					HitDirection.Z = 0.0f;
					HitDirection.Normalize();
					HitComp->SetHitDirection(HitDirection);
					HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());

					//1.2 Damage Setting
					if (IsLastCombo() == true)
					{
						//@마무리 일격을 위한 데미지 세팅
						DamageType->SetDamageImpulse(TargetInfoWidget->GetInfoMonsterCurrentHP() + 0.1f);
					}

					//1.3 UI 표기 X
					IIC_WidgetInfo* I_Widget = Cast<IIC_WidgetInfo>(HitResult.GetActor());
					if (I_Widget != nullptr)
					{
						I_Widget->SetOnceNoneUsingFloatingCombo();
					}

					// 1.4 OnHit Call
					HitComp->OnHit(Player, DamageType, DamageType->DamageImpulse);

					//if (IsLastCombo() == true)
					//{
					//	//@마지막 일격 - StrongAttack
					//	FVector HitDirection = Player->GetActorForwardVector();
					//	HitDirection.Z = 0.0f;
					//	HitComp->SetHitDirection(HitDirection);
					//	HitComp->SetHitMoveSpeed(0.3f);
					//	HitComp->OnHit(Player, DT_StrongAttack, DT_StrongAttack->DamageImpulse);
					//}
					//else
					//{
					//	FVector HitDirection = Player->GetActorForwardVector();
					//	HitDirection.Z = 0.0f;
					//	HitComp->SetHitDirection(HitDirection);
					//	HitComp->SetHitMoveSpeed(0.0f);
					//	HitComp->OnHit(Player, DT_Noraml, 0.0f);
					//}

				}//(HitComp != nullptr)
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
			}//(Charactor != nullptr)
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");

		}//for(HitResult)
	}//(bHit == true)
}
