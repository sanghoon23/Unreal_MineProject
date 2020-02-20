#include "CPL_MGAttackBasic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Charactor/Player/CPlayer.h"

UCPL_MGAttackBasic::UCPL_MGAttackBasic()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = static_cast<UINT>(UMG_BasicAttack::COMBO_ONE);
		MaxComboNum = static_cast<UINT>(UMG_BasicAttack::END);

		AttackRange = 230.0f;
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
	DT_Noraml = NewObject<UCDamageType_Normal>();
	DT_StrongAttack = NewObject<UCDamageType_StrongAttack>();

	#pragma endregion
}

void UCPL_MGAttackBasic::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_MGAttackBasic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IBaseAttack ����.
// @param DoingActor - �����ϴ� ��ü
// ���� Anim �� ������ true �� ����
// �׷��� ������ Input �����ʹ� �ٲ�µ�, 
// ��Ÿ�ְ� ������� �ʾ� Output �����Ͱ� ���µ�������.
void UCPL_MGAttackBasic::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		bAttackCall = true;
		IfFalseRet(bAttackPossible); // @Super::Tick ���� ó�� ��.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

	//@ Ÿ�� �ٶ󺸰� �ϱ�
	LookAtTarget(Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			MageAttackMontages[0],	/* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay ������ ����.
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
	check(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bComboCheck = false;
	++CurrentComboNum;

	// @Ÿ�� �ٶ󺸰� �ϱ�
	LookAtTarget(Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	// ���� �˻�.( CurrentComboNum = 0 ~ MaxCombo ���� )
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

// @Combo �� ������ ������ ��Ȯ�� �˱� ���ؼ�.
bool UCPL_MGAttackBasic::IsLastCombo() const
{
	if (CurrentComboNum == static_cast<uint8>(UMG_BasicAttack::COMBO_THREE))
		return true;

	return false;
}

void UCPL_MGAttackBasic::AttackOtherPawn()
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
				HitComp->SetHitDirection(HitDirection);
				HitComp->SetHitMoveSpeed(0.3f);

				// 1.2 Hit Delegate - Normal(DamageType)
				if (CurrentComboNum < static_cast<uint8>(UMG_BasicAttack::COMBO_THREE))
				{
					HitComp->OnHit(Player, DT_Noraml, 50.0f);
				}
				else if (CurrentComboNum == static_cast<uint8>(UMG_BasicAttack::COMBO_THREE))
				{
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

void UCPL_MGAttackBasic::LookAtTarget(AActor * Target)
{
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}
