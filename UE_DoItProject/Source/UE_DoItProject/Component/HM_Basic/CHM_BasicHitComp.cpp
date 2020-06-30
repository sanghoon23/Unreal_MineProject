#include "CHM_BasicHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Basic.h"
#include "DamageType/Base/CDamageType_Base.h"

UCHM_BasicHitComp::UCHM_BasicHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

#pragma region Hit Montages
	//@Super
	{
		// 'CanHitCom' Montage
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_HitCombo.HM_Basic_HitCombo'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> comboHit(*Path);
		if (comboHit.Succeeded())
			HitComboMon = comboHit.Object;
	}

	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Basic/HM_Basic_NormalHit1.HM_Basic_NormalHit1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'AirHitFirst' Hit Montage
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

#pragma region Poision Material
	//@LOAD Poision Material
	{
		Path = L"Material'/Game/_Mine/Mesh/HM_Basic/CharM_Standard/M_Char_Standard_Poision.M_Char_Standard_Poision'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> PoisionMat(*Path);
		if (PoisionMat.Succeeded())
		{
			Mat_Poision_0 = PoisionMat.Object;
		}
	}
#pragma endregion

	//@LOAD Stun Head Particle
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_StunActor.PS_StunActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_StunHead(*Path);
		if (P_StunHead.Succeeded())
		{
			StunHeadParticle = P_StunHead.Object;
		}
	}

	//@LOAD Burn Particle - ParticleComp
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_BurningActor.PS_BurningActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> BurnPT(*Path);
		if (BurnPT.Succeeded())
		{
			BurnParticle = BurnPT.Object;
		}
	}

	//@LOAD Freeze Particle - ParticleComp
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_FreezingActor.PS_FreezingActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> FreezePT(*Path);
		if (FreezePT.Succeeded())
		{
			FreezeParticle = FreezePT.Object;
		}
	}
}

void UCHM_BasicHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_Basic = Cast<ACHM_Basic>(GetOwner());
	check(HM_Basic);

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bDamaged = true; //@다른 몽타주가 실행되기 때문에
	});

	//@Set Poision Material
	{
		Map_ChangePoisionMaterial.Add(0, Mat_Poision_0);
		Map_OriginPoisionMaterial.Add(0, HM_Basic->GetMesh()->GetMaterial(0));
	}
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

	IfFalseRet(bCanAttackFromOther);

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate 실행.
	HM_Basic->OnActionResetState.Broadcast(HM_Basic);

	///DamageType Process
	//if (IsDamagedFromOther() == true)
	//{
	//	Type->OnHittingProcess(AttackingActor, HM_Basic, this, DamageAmount);
	//}

	Type->OnHittingProcess(AttackingActor, HM_Basic, this, DamageAmount);

	////@Montage 실행 - bBlockDamageMontage 변수 여부 ( BaseHitComp )
	//IfTrueRet(bBlockDamagedMontage);

	////@콤보가 가능한지,
	//if (bCanHitCombo == true)
	//{
	//	if (HitComboMon != nullptr)
	//	{
	//		SetCanHittedCombo(false); //@false
	//		HM_Basic->ActorAnimMonPlay(HitComboMon, 0.6f, true);
	//		CLog::Print(L"Can Hit Combo!!");
	//		return; //@return
	//	}
	//}

	////@else
	//const uint8 MontageNum = static_cast<uint8>(Type->GetConditionType());
	//if (MontageNum >= DamagedMontages.Num())
	//{
	//	UE_LOG(LogTemp, Warning, L"HitComp MontageNumber EXCEED!!");
	//	return;
	//}
	//UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
	//if (RunMontage != nullptr)
	//{
	//	HM_Basic->ActorAnimMonPlay(RunMontage, 0.6f, true);
	//}
}
