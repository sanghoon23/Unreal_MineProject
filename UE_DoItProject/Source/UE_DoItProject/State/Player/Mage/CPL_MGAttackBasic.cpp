#include "CPL_MGAttackBasic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"

UCPL_MGAttackBasic::UCPL_MGAttackBasic()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(UMG_BasicAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(UMG_BasicAttack::END);
	}

	FString Path = L"";
	#pragma region BasicAttack
	// Setting
	{
		UAnimMontage* Mage_BasicAttack_1 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_BasicAttack1.Mage_BasicAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_1(*Path);
		if (MBasicAttack_1.Succeeded())
			Mage_BasicAttack_1 = MBasicAttack_1.Object;

		UAnimMontage* Mage_BasicAttack_2 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_BasicAttack2.Mage_BasicAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_2(*Path);
		if (MBasicAttack_2.Succeeded())
			Mage_BasicAttack_2 = MBasicAttack_2.Object;

		UAnimMontage* Mage_BasicAttack_3 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MAttack/Mage_BasicAttack3.Mage_BasicAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBasicAttack_3(*Path);
		if (MBasicAttack_3.Succeeded())
			Mage_BasicAttack_3 = MBasicAttack_3.Object;

		MageAttackMontages.Emplace(Mage_BasicAttack_1);
		MageAttackMontages.Emplace(Mage_BasicAttack_2);
		MageAttackMontages.Emplace(Mage_BasicAttack_3);
	}

	#pragma endregion

	#pragma region Create DamageType

	DT_Normal = NewObject<UCDamageType_Normal>();
	DT_StrongAttack = NewObject<UCDamageType_StrongAttack>();

	#pragma endregion
}

void UCPL_MGAttackBasic::BeginPlay()
{
	Super::BeginPlay();
	
	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion
}

void UCPL_MGAttackBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MGAttackBasic::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

// - IBaseAttack 참고.
// @param DoingActor - 공격하는 주체
// 공격 Anim 은 무조건 true 로 실행
// 그렇지 않으면 Input 데이터는 바뀌는데, 
// 몽타주가 실행되지 않아 Output 데이터가 리셋되지않음.
void UCPL_MGAttackBasic::BeginAttack(AActor * DoingActor)
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
	IfTrueRet(Player->GetCharacterMovement()->IsFalling() && Player->GetCharacterMovement()->GravityScale > 0.0f); //@InAir
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	if (Target != nullptr)
	{
		//@ 타겟 바라보게 하기
		UCFL_ActorAgainst::LookAtTarget(Player, Target);
	}

	// @공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0],	/* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay 무조건 실행.
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

void UCPL_MGAttackBasic::OnComboSet(AActor * DoingActor)
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
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[CurrentComboNum], 
			1.0f, false
		);
	}
	else
	{
		EndAttack();
	}
}

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCPL_MGAttackBasic::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(UMG_BasicAttack::COMBO_THREE))
		return true;

	return false;
}

void UCPL_MGAttackBasic::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackRange;

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	FHitResult HitResult;
	float DebugLifeTime = 1.0f;
	bool bHit = GetWorld()->SweepSingleByChannel //@Single - 단일.
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
				HitComp->SetHitDirection(HitDirection);
				HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());

				HitComp->OnHit(Player, DamageType, DamageType->DamageImpulse);

			}//(HitComp != nullptr)
			else
				UE_LOG(LogTemp, Warning, L"MGAttackBasic CallAttack - HitComp Null!!");
		}//(Charactor != nullptr)
		else
			UE_LOG(LogTemp, Warning, L"MGAttackBasic CallAttack - Charactor Null!!");
	}//(bHit == true)
}
