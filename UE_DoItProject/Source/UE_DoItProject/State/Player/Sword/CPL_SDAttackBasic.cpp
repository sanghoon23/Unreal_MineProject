#include "CPL_SDAttackBasic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "DamageType/CDamageType_Normal.h"

UCPL_SDAttackBasic::UCPL_SDAttackBasic()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ComboSetting
	{
		CurrentComboNum = static_cast<UINT>(USD_BasicAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(USD_BasicAttack::END);
	}

	FString Path = L"";

	#pragma region SwordAttackMontage
	// SwordAttackMontage
	{
		UAnimMontage* Sword_BasicAttack_1 = nullptr;
		UAnimMontage* Sword_BasicAttack_2 = nullptr;
		UAnimMontage* Sword_BasicAttack_3 = nullptr;

		// UINT Sword_Type = static_cast<UINT>(SwordAttackType::BASIC);

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

	#pragma region DamageType
	//// @'Normal' DamageType
	//Path = L"Blueprint'/Game/_Mine/_MyBlueprint/DamageType/BpCDamageType_Normal.BpCDamageType_Normal_C'";
	//ConstructorHelpers::FClassFinder<UCDamageType_Normal> Normal(*Path);
	//if (Normal.Succeeded())
	//	DT_Normal = Normal.Class;

	DT_Noraml = NewObject<UCDamageType_Normal>();

	#pragma endregion
}

void UCPL_SDAttackBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackBasic::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_SDAttackBasic::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// @AttackCall
	bAttackCall = true;

	//@공격 가능할 때까지 Return - Super::Tick 에서 처리 중.
	IfFalseRet(bAttackPossible);

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	//@캐릭터 이동불가.
	// Player->CanNotMove();

	//@ 타겟 바라보게 하기
	LookAtFindedTarget();


	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, false
		);
	}
	else if (bAttacking == true)
	{
		bComboCheck = true;
	}
}

void UCPL_SDAttackBasic::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);
	check(DoingActor);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(DoingActor);
	check(Charactor);

	bComboCheck = false;
	++CurrentComboNum;

	//@ 타겟 바라보게 하기
	LookAtFindedTarget();

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

// @DoingActor - Attack 을 할 객체 즉, 여기선 Player (Owner)
void UCPL_SDAttackBasic::AttackOtherPawn()
{
	Super::AttackOtherPawn();

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + ActorForward * AttackDistance;

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

	if (bHit)
	{
		IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
		if (Charactor != nullptr)
		{
			// HitComp 가 없을 수도 있기 때문에
			// 1. Get Interface HitComp
			IIC_HitComp* HitComp = Charactor->GetIHitComp();
			if (HitComp != nullptr)
			{
				// 1.1 Set Hit Attribute
				HitComp->SetHitDirection(Player->GetActorForwardVector());
				HitComp->SetHitMoveSpeed(3.0f);

				// 1.2 Hit Delegate - Normal(DamageType)
				HitComp->OnHit(Player, DT_Noraml, 50.0f);
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
}

void UCPL_SDAttackBasic::CheckProcedural()
{
	Super::CheckProcedural();
}

/* TargetSystem 의 FindTarget 바라보게 하기 */
void UCPL_SDAttackBasic::LookAtFindedTarget()
{
	APawn* Target = Player->GetFindAttackTarget();
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}
