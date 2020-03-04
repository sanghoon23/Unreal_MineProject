#include "CPL_SDAttackUpper.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"

UCPL_SDAttackUpper::UCPL_SDAttackUpper()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(USD_UpperAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(USD_UpperAttack::END);

		AttackRange = 230.0f;
	}

	FString Path = L"";
	#pragma region Sword UpperAttack Montages
	// Sword UpperAttack Montages
	{
		// @1
		UAnimMontage* Sword_UpperAttack_1 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack1.SwordMon_UpperAttack1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_1(*Path);
		if (MSecondAttack_1.Succeeded())
			Sword_UpperAttack_1 = MSecondAttack_1.Object;

		// @2
		UAnimMontage* Sword_UpperAttack_2 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack2.SwordMon_UpperAttack2'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_2(*Path);
		if (MSecondAttack_2.Succeeded())
			Sword_UpperAttack_2 = MSecondAttack_2.Object;

		// @3
		UAnimMontage* Sword_UpperAttack_3 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack3.SwordMon_UpperAttack3'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_3(*Path);
		if (MSecondAttack_3.Succeeded())
			Sword_UpperAttack_3 = MSecondAttack_3.Object;

		// @4
		UAnimMontage* Sword_UpperAttack_4 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack4.SwordMon_UpperAttack4'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_4(*Path);
		if (MSecondAttack_4.Succeeded())
			Sword_UpperAttack_4 = MSecondAttack_4.Object;

		// @5
		UAnimMontage* Sword_UpperAttack_5 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack5.SwordMon_UpperAttack5'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_5(*Path);
		if (MSecondAttack_5.Succeeded())
			Sword_UpperAttack_5 = MSecondAttack_5.Object;

		// @6
		UAnimMontage* Sword_UpperAttack_6 = nullptr;
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_UpperAttack6.SwordMon_UpperAttack6'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSecondAttack_6(*Path);
		if (MSecondAttack_6.Succeeded())
			Sword_UpperAttack_6 = MSecondAttack_6.Object;

		SwordAttackMontages.Emplace(Sword_UpperAttack_1);
		SwordAttackMontages.Emplace(Sword_UpperAttack_2);
		SwordAttackMontages.Emplace(Sword_UpperAttack_3);
		SwordAttackMontages.Emplace(Sword_UpperAttack_4);
		SwordAttackMontages.Emplace(Sword_UpperAttack_5);
		SwordAttackMontages.Emplace(Sword_UpperAttack_6);
	}

	#pragma endregion

	#pragma region DamageType

	DT_Air = NewObject<UCDamageType_Air>();
	DT_AirAttack = NewObject<UCDamageType_AirAttack>();
	DT_StrongAttack = NewObject<UCDamageType_StrongAttack>();

	#pragma endregion

}

void UCPL_SDAttackUpper::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackUpper::BeginPlay()
{
	Super::BeginPlay();
}

// - IBaseAttack 참고.
// @param DoingActor - 공격하는 주체
// #Edit *0220
// @Warning - 공격 Anim 은 무조건 true 로 실행
// 연속적으로 InputKey 가 들어올때,
// 이전의 몽타주가 현재의 몽타주의 bAttacking == false 로 만듬.(ComboNotify 의 EndAttack 이)
void UCPL_SDAttackUpper::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		bAttackCall = true;
		IfFalseRet(bAttackPossible); // @Super::Tick 에서 처리 중.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

	//@ 타겟 바라보게 하기
	LookAtTarget(Target);

	// @공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 0.7f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.3f, true				// @AnimPlay 무조건 실행.
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}

}

// - IBaseAttack 참고.
void UCPL_SDAttackUpper::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bComboCheck = false;
	++CurrentComboNum;

	// @Combo 시 Target 위치 앞에서 공격하기
	// 2번째 콤보일 때만,
	// #Edit *0220 - 가까이서 올려버리면, 높이을 맞추다가 Pawn 끼리 충돌 나서 공중에 뜨질 못함.
	if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_TWO))
	{
		ActorLocateFrontTarget(Target); // @거리 벌리고, 높이 맞추기
	}

	// @Input 고정
	Player->CanNotMove();

	// @타겟 바라보게 하기
	LookAtTarget(Target);

	// @중력 끄기
	Charactor->OffGravity();

	// @공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 1.1f;

	// 조건 검사.( CurrentComboNum = 0 ~ MaxCombo 까지 )
	CurrentComboNum = FMath::Clamp<UINT>(CurrentComboNum, 0, MaxComboNum);
	if (CurrentComboNum < MaxComboNum)
	{
		Charactor->ActorAnimMonPlay
		(
			SwordAttackMontages[CurrentComboNum],
			1.8f, false
		);
	}
	else
	{
		EndAttack();
	}
}

// @Combo 의 마지막 구간을 정확히 알기 위해서.
bool UCPL_SDAttackUpper::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
		return true;

	return false;
}


/* 다른 Pawn 을 공격 처리 함수 */
// @DoingActor - Attack 을 할 객체 즉, 여기선 Player (Owner)
void UCPL_SDAttackUpper::AttackOtherPawn()
{
	Super::AttackOtherPawn();

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

				// 1.2 Hit Delegate - Air(DamageType)
				if (CurrentComboNum == 0)
				{
					HitComp->SetHitMoveSpeed(0.3f);
					HitComp->OnHit(Player, DT_Air, 50.0f);
				}
				else if (CurrentComboNum > 0 && CurrentComboNum < static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
				{
					HitComp->SetHitMoveSpeed(0.3f);
					HitComp->OnHit(Player, DT_AirAttack, 50.0f);
				}
				else if (CurrentComboNum == static_cast<uint8>(USD_UpperAttack::COMBO_SIX))
				{
					HitComp->SetHitMoveSpeed(500.0f);
					HitComp->OnHit(Player, DT_StrongAttack, 50.0f);
				}

			}//(HitComp != nullptr)
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - HitComp Null!!");
		}//(Charactor != nullptr)
		else
			UE_LOG(LogTemp, Warning, L"SDAttackBasic CallAttack - Charactor Null!!");
	}//(bHit == true)
}

/* TargetSystem Target 을 바라보도록 함 */
void UCPL_SDAttackUpper::LookAtTarget(AActor* Target)
{
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}

// #Edit *0220 / 02:31
/* 거리까지 맞춰줌. */
/* TargetSystem Target 의 높이 와 Player 의 높이를 동일하게 맞춤. */
void UCPL_SDAttackUpper::ActorLocateFrontTarget(AActor * Target)
{
	check(Target);
	FVector TargetLocation = Target->GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();
	FVector LookDir = TargetLocation - PlayerLocation;
	LookDir.Normalize();
	LookDir.Z = 0.0f;

	// @Target 에서 AttackRange 만큼 떨어진 곳으로 위치시키기 위해
	FVector SettingLocation = TargetLocation;

	// @LookDir 을 이용해 , 거리 맞추고
	SettingLocation += (-1) * LookDir * (AttackRange + 0.0f);

	// @Target 의 높이에 맞추고,
	SettingLocation.Z = TargetLocation.Z;

	// @Setting Location
	Player->SetActorLocation(SettingLocation);
}

