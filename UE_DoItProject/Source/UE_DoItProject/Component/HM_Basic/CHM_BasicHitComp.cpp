#include "CHM_BasicHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Basic.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Air.h"
#include "DamageType/CDamageType_AirAttack.h"
#include "DamageType/CDamageType_Stun.h"
#include "DamageType/CDamageType_Burn.h"
#include "DamageType/CDamageType_Poision.h"
#include "DamageType/CDamageType_Freeze.h"

#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Stun.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Burn.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Poision.h"
#include "DamagedConditionType/UpsetCondition/CUpset_Freeze.h"

UCHM_BasicHitComp::UCHM_BasicHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

	//Create Common Death Montage
	Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_BasicMon_Die01.HM_BasicMon_Die01'";
	ConstructorHelpers::FObjectFinder<UAnimMontage> DeathMon(*Path);
	if (DeathMon.Succeeded())
	{
		CommonDeathMontage = DeathMon.Object;
	}

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

	#pragma region Set MontagesArray - (BaseHitComp)
	//@Set Montage
	//...

	uint8 NormalNum = static_cast<uint8>(FDamageType::NORMAL);
	DamagedMontages[NormalNum] = NormalHitMontage;

	uint8 AirNum = static_cast<uint8>(FDamageType::AIR);
	DamagedMontages[AirNum] = AirHitMontage;

	uint8 AirAttackNum = static_cast<uint8>(FDamageType::AIRATTACK);
	DamagedMontages[AirAttackNum] = AirAttackHitMontage;

	uint8 StrongAttackNum = static_cast<uint8>(FDamageType::STRONGATTACK);
	DamagedMontages[StrongAttackNum] = StrongAttackHitMontage;

	uint8 StunNum = static_cast<uint8>(FDamageType::STUN);
	DamagedMontages[StunNum] = StunHitMontage;

	//uint8 FreezeNum = static_cast<uint8>(FDamageType::FREEZE);
	//DamagedMontages[FreezeNum] = NormalHitMontage;

	#pragma endregion

	//@Load Poision Material
	{
		Path = L"Material'/Game/_Mine/Mesh/HM_Basic/CharM_Standard/M_Char_Standard_Poision.M_Char_Standard_Poision'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> PoisionMat(*Path);
		if (PoisionMat.Succeeded())
		{
			PoisionMaterial = PoisionMat.Object;
		}
	}

	//@Load Burn Particle - ParticleComp
	{
		BurnParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("BurnParticleComp");

		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_BurningActor.PS_BurningActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> BurnPT(*Path);
		if (BurnPT.Succeeded())
		{
			BurnParticleComp->bAutoActivate = false;
			BurnParticleComp->SetTemplate(BurnPT.Object);
		}
	}

	//@Load Freeze Particle - ParticleComp
	{
		FreezeParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("FreezeParticleComp");

		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_FreezingActor.PS_FreezingActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> FreezePT(*Path);
		if (FreezePT.Succeeded())
		{
			FreezeParticleComp->bAutoActivate = false;
			FreezeParticleComp->SetTemplate(FreezePT.Object);
		}
	}
}

void UCHM_BasicHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_Basic = Cast<ACHM_Basic>(GetOwner());
	AnimInst_HM_Basic = HM_Basic->GetMesh()->GetAnimInstance();

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

void UCHM_BasicHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate 실행.
	HM_Basic->OnActionResetState.Broadcast(HM_Basic);

	///DamageType Process
	Type->OnHittingProcess(AttackingActor, HM_Basic, this, DamageAmount);

	//#Edit 0510 - 
	//@Death Animation 은 AnimInstance 의 LocoMotion 을 이용

	//@Montage 실행 - bBlockDamageMontage 변수 여부 ( BaseHitComp )
	IfTrueRet(bBlockDamagedMontage);

	const uint8 MontageNum = static_cast<uint8>(Type->GetConditionType());
	if (MontageNum >= DamagedMontages.Num()) return;
	UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
	if (RunMontage != nullptr)
	{
		HM_Basic->ActorAnimMonPlay(RunMontage, 0.6f, true);
	}
}
