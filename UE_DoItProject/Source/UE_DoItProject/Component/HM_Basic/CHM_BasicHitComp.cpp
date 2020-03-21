#include "CHM_BasicHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Basic.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "DamageType/CDamageType_Stun.h"

UCHM_BasicHitComp::UCHM_BasicHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";
	#pragma region Hit Montages
	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_NormalHit1.HM_Basic_NormalHit1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHit = normalHit.Object;
	}

	// 'Air' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_AirHitFirst.HM_Basic_AirHitFirst'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airHit(*Path);
		if (airHit.Succeeded())
			AirHit = airHit.Object;
	}

	// 'AirAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_AirAttackHit.HM_Basic_AirAttackHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airAttackHit(*Path);
		if (airAttackHit.Succeeded())
			AirAttackHit = airAttackHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_StrongAttack.HM_Basic_StrongAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHit = strongAttackHit.Object;
	}

	// 'Stun' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_Stun_.HM_Basic_Stun_'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> stunHit(*Path);
		if (stunHit.Succeeded())
			StunHit = stunHit.Object;
	}


	#pragma endregion
}

void UCHM_BasicHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_Basic = Cast<ACHM_Basic>(GetOwner());

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		HM_Basic->OnGravity(); //@�߷�Ű��
	});
}


void UCHM_BasicHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_BasicHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * DamageType, float DamageAmount)
{
	Super::OnHit(AttackingActor, DamageType, DamageAmount);
	check(AttackingActor);
	check(DamageType);

	// @Interface
	IIC_Charactor* ICharactor = Cast<IIC_Charactor>(HM_Basic);
	check(ICharactor);

	//1. Delegate ����.
	// @ResetState
	HM_Basic->OnActionResetState.Broadcast(HM_Basic);

	// @NORMAL - �Ϲ� ����
	if (DamageType->GetConditionType() == FConditionType::NORMAL)
	{
		// @OnDamageDelegate
		// DamageType->OnDamageDelegate(Owner);

		// @���� ��� �ٶ󺸱�
		LookAtActor(AttackingActor);

		//1.3 TakeDamage


		//1.4 �ִϸ��̼� ���� - (������ ����)
		HM_Basic->ActorAnimMonPlay(NormalHit, 0.6f, true);

		//CLog::Print(L"TYPE - NORMAL");
	}
	
	// @AIR - ����
	else if (DamageType->GetConditionType() == FConditionType::AIR)
	{
		// @���� ��� �ٶ󺸱�
		LookAtActor(AttackingActor);

		//1.3 TakeDamage

		//1.4 Jump
		ACharacter* Charactor = Cast<ACharacter>(HM_Basic);
		check(Charactor);
		FVector Location = Charactor->GetActorLocation();
		Location.Z += 200.0f;
		Charactor->SetActorLocation(Location);

		// @�ӷ� ���̱� - �߷²��� �ٷ� �������
		HM_Basic->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @�߷� ����.
		HM_Basic->OffGravity();

		//1.6 �ִϸ��̼� ���� - (������ ����)
		HM_Basic->ActorAnimMonPlay(AirHit, 0.6f, true);

		//CLog::Print(L"TYPE - AIR");
	}

	// @AIR ATTACK
	else if (DamageType->GetConditionType() == FConditionType::AIRATTACK)
	{
		// @���� ��� �ٶ󺸱�
		LookAtActor(AttackingActor);

		//1.3 TakeDamage

		// @�ӷ� ���̱� - �߷²��� �ٷ� �������
		HM_Basic->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @�߷� ����.
		HM_Basic->OffGravity();

		//1.5 �ִϸ��̼� ���� - (������ ����)
		HM_Basic->ActorAnimMonPlay(AirAttackHit, 0.6f, true);

		//CLog::Print(L"TYPE - AIRATTACK");
	}

	// @STRONG ATTACK
	else if (DamageType->GetConditionType() == FConditionType::STRONGATTACK)
	{
		// @���� ��� �ٶ󺸱�
		LookAtActor(AttackingActor);

		//@�ִϸ��̼� ���� - (������ ����)
		HM_Basic->ActorAnimMonPlay(StrongAttackHit, 0.6f, true);
	}

	// @STUN ATTACK
	else if (DamageType->GetConditionType() == FConditionType::STUN)
	{
		//@���� ��� �ٶ󺸱�
		LookAtActor(AttackingActor);

		//@StunType ĳ����
		UCDamageType_Stun* StunType = Cast<UCDamageType_Stun>(DamageType);
		check(StunType);

		//@�����̻� �߰� - BaseHitComp
		FHitNonActionConditionData ConditionData;
		ConditionData.ApplyTime = StunType->GetStunTime();
		ConditionData.NonActionMon = StunHit;
		AddNonActionCondition(ConditionData);

		//@�ִϸ��̼� ���� - (������ ����)
		HM_Basic->ActorAnimMonPlay(StunHit, 0.6f, true);
	}
}

// @param Target - �ٶ� ���
void UCHM_BasicHitComp::LookAtActor(AActor * Target)
{
	FVector LookVec = Target->GetActorLocation() - HM_Basic->GetActorLocation();
	LookVec.Z = 0.0f;
	FRotator LookRot = FRotationMatrix::MakeFromX(LookVec).Rotator();
	HM_Basic->SetActorRotation(FRotator(0.0f, LookRot.Yaw, 0.0f)); // OK 
}
