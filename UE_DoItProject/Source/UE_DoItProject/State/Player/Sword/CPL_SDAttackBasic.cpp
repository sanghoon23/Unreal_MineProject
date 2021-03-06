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

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordAnim_BasicAttack_1_Montage.SwordAnim_BasicAttack_1_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_1(*Path);
		if (MBasicAttack_1.Succeeded())
			Sword_BasicAttack_1 = MBasicAttack_1.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordAnim_BasicAttack_2_Montage.SwordAnim_BasicAttack_2_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_2(*Path);
		if (MBasicAttack_2.Succeeded())
			Sword_BasicAttack_2 = MBasicAttack_2.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordAnim_BasicAttack_3_Montage.SwordAnim_BasicAttack_3_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_3(*Path);
		if (MBasicAttack_3.Succeeded())
			Sword_BasicAttack_3 = MBasicAttack_3.Object;

		SwordAttackMontages.Emplace(Sword_BasicAttack_1);
		SwordAttackMontages.Emplace(Sword_BasicAttack_2);
		SwordAttackMontages.Emplace(Sword_BasicAttack_3);
	}
	#pragma endregion
}

void UCPL_SDAttackBasic::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

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

// - IBaseAttack 참고.
// @param DoingActor - 공격하는 주체
// @Warning - 공격 Anim 은 무조건 true 로 실행
// 연속적으로 InputKey 가 들어올때,
// 이전의 몽타주가 현재의 몽타주의 bAttacking == false 로 만듬.(ComboNotify 의 EndAttack 이)
void UCPL_SDAttackBasic::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick 에서 처리 중.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	if (Target != nullptr)
	{
		//@ 타겟 바라보게 하기
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
	//// @타겟 바라보게 하기
	//UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay 무조건 실행.
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

// - IBaseAttack 참고.
void UCPL_SDAttackBasic::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	APawn* Target = Player->GetFindAttackTarget();
	if (Target != nullptr)
	{
		//@ 타겟 바라보게 하기
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

	//	//@ 타겟 바라보게 하기
	//UCFL_ActorAgainst::LookAtTarget(Player, Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bComboCheck = false;
	++CurrentComboNum;

	// @공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	// 조건 검사.( CurrentComboNum = 0 ~ MaxCombo 까지 )
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

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCPL_SDAttackBasic::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(USD_BasicAttack::COMBO_THREE))
		return true;

	return false;
}

/* 다른 Pawn 을 공격 처리 함수 */
void UCPL_SDAttackBasic::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	TArray<FHitResult> HitResults;
	float DebugLifeTime = 1.0f;
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

	//DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		AActor* const Target = Player->GetFindAttackTarget();
		for (auto& HitResult : HitResults)
		{
			if (Target != nullptr)
			{
				if (Target == HitResult.GetActor())
				{
					IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
					if (Charactor != nullptr)
					{
						// 1. Get Interface HitComp
						IIC_HitComp* HitComp = Charactor->GetIHitComp();
						if (HitComp != nullptr)
						{
							// 1.1 Set Hit Attribute
							FVector HitDirection = HitResult.GetActor()->GetActorLocation() - Player->GetActorLocation();
							HitDirection.Z = 0.0f;
							HitDirection.Normalize();
							HitComp->SetHitDirection(HitDirection);
							HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());

							HitComp->OnHit(Player, DamageType, DamageType->DamageImpulse);

						}//(HitComp != nullptr)
						else
							UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
					}//(Charactor != nullptr)
					else
						UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
				}
				else continue;
			}
			else //(Target==nullptr)
			{
				IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
				if (Charactor != nullptr)
				{
					// 1. Get Interface HitComp
					IIC_HitComp* HitComp = Charactor->GetIHitComp();
					if (HitComp != nullptr)
					{
						// 1.1 Set Hit Attribute
						FVector HitDirection = HitResult.GetActor()->GetActorLocation() - Player->GetActorLocation();
						HitDirection.Z = 0.0f;
						HitDirection.Normalize();
						HitComp->SetHitDirection(HitDirection);
						HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());

						HitComp->OnHit(Player, DamageType, DamageType->DamageImpulse);

					}//(HitComp != nullptr)
					else
						UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
				}//(Charactor != nullptr)
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
			}
		}
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
	bool bHit = GetWorld()->SweepMultiByChannel //@충격량 주기 - Multi
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
			if (Hit.GetActor()->IsA<APawn>()) // Pawn 이면, Continue;
				continue;

			if (Cast<ADestructibleActor>(Hit.GetActor()))
			{
				UDestructibleComponent* destructible
					= Cast<UDestructibleComponent>(Hit.GetActor()->GetComponentByClass(UDestructibleComponent::StaticClass()));
				if (destructible != nullptr)
				{
					destructible->ApplyDamage(5.0f, Hit.GetActor()->GetActorLocation(), ActorForward, 1.0f);
				}
				continue;
			}

			//ex) ItemBox_MediumWood..
			FDamageEvent DamageEvent;
			Hit.GetActor()->TakeDamage(1.0f, DamageEvent, Player->GetController(), Hit.GetActor());
		}
	}//(bHit)
}

void UCPL_SDAttackBasic::CheckProcedural()
{
	Super::CheckProcedural();
}
