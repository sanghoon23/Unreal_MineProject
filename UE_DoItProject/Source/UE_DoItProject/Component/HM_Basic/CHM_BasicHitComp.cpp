#include "CHM_BasicHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

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
			NormalHitMontage = normalHit.Object;
	}

	// 'Air' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_AirHitFirst.HM_Basic_AirHitFirst'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airHit(*Path);
		if (airHit.Succeeded())
			AirHitMontage = airHit.Object;
	}

	// 'AirAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_AirAttackHit.HM_Basic_AirAttackHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airAttackHit(*Path);
		if (airAttackHit.Succeeded())
			AirAttackHitMontage = airAttackHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_StrongAttack.HM_Basic_StrongAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

	// 'Stun' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_Stun_.HM_Basic_Stun_'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> stunHit(*Path);
		if (stunHit.Succeeded())
			StunHitMontage = stunHit.Object;
	}


	#pragma endregion

	//// @Creat Condition Data
	//StunNonActionData = NewObject<UHitNonActionConditionData>();
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
		HM_Basic->OnGravity(); //@중력키기
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

	//1. Delegate 실행.
	// @ResetState
	HM_Basic->OnActionResetState.Broadcast(HM_Basic);

	// @NORMAL - 일반 공격
	if (DamageType->GetConditionType() == FConditionType::NORMAL)
	{
		// @OnDamageDelegate
		// DamageType->OnDamageDelegate(Owner);

		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//1.3 TakeDamage


		//1.4 애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(NormalHitMontage, 0.6f, true);

		//CLog::Print(L"TYPE - NORMAL");
	}
	
	// @AIR - 띄우기
	else if (DamageType->GetConditionType() == FConditionType::AIR)
	{
		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//1.3 TakeDamage

		//1.4 Jump
		ACharacter* Charactor = Cast<ACharacter>(HM_Basic);
		check(Charactor);
		FVector Location = Charactor->GetActorLocation();
		Location.Z += 200.0f;
		Charactor->SetActorLocation(Location);

		// @속력 줄이기 - 중력끄고 바로 해줘야함
		HM_Basic->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @중력 끄기.
		HM_Basic->OffGravity();

		//1.6 애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(AirHitMontage, 0.6f, true);

		//CLog::Print(L"TYPE - AIR");
	}

	// @AIR ATTACK
	else if (DamageType->GetConditionType() == FConditionType::AIRATTACK)
	{
		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//1.3 TakeDamage

		// @속력 줄이기 - 중력끄고 바로 해줘야함
		HM_Basic->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @중력 끄기.
		HM_Basic->OffGravity();

		//1.5 애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(AirAttackHitMontage, 0.6f, true);

		//CLog::Print(L"TYPE - AIRATTACK");
	}

	// @STRONG ATTACK
	else if (DamageType->GetConditionType() == FConditionType::STRONGATTACK)
	{
		// @때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//@애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(StrongAttackHitMontage, 0.6f, true);
	}

	// @STUN ATTACK
	else if (DamageType->GetConditionType() == FConditionType::STUN)
	{
		//@때린 대상 바라보기
		UCFL_ActorAgainst::LookAtTarget(HM_Basic, AttackingActor);

		//@StunType 캐스팅
		UCDamageType_Stun* StunType = Cast<UCDamageType_Stun>(DamageType);
		check(StunType);

		//@상태이상 추가 - BaseHitComp
		StunNonActionData = NewObject<UHitNonActionConditionData>();
		StunNonActionData->ApplyTime = StunType->GetStunTime();
		StunNonActionData->NonActionMon = StunHitMontage;
		if (StunType->StunConditionUITexture != nullptr)
		{
			StunNonActionData->TextureUI = StunType->StunConditionUITexture;
		}
		AddConditionData(StunNonActionData);

		//@애니메이션 실행 - (무조건 실행)
		HM_Basic->ActorAnimMonPlay(StunHitMontage, 0.6f, true);
	}
}
