#include "CHM_AssassinHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Assassin.h"
#include "DamageType/Base/CDamageType_Base.h"

UCHM_AssassinHitComp::UCHM_AssassinHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

#pragma region Hit Montages

	//@Super
	{
		// 'CanHitCom' Montage
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_HitCombo.HM_Assa_Mon_HitCombo'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> comboHit(*Path);
		if (comboHit.Succeeded())
			HitComboMon = comboHit.Object;
	}

	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_NormalHit_Left.HM_Assa_Mon_NormalHit_Left'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'AirHitFirst' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_AirHitFirst.HM_Assa_Mon_AirHitFirst'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airHit(*Path);
		if (airHit.Succeeded())
			AirHitMontage = airHit.Object;
	}

	// 'AirAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_AirAttackHit.HM_Assa_Mon_AirAttackHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airAttackHit(*Path);
		if (airAttackHit.Succeeded())
			AirAttackHitMontage = airAttackHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_StrongAttack.HM_Assa_Mon_StrongAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

#pragma endregion

	//@Set Montage
	uint8 NormalNum = static_cast<uint8>(FDamageType::NORMAL);
	bUsingDamageTypeEffect[NormalNum] = true;
	DamagedMontages[NormalNum] = NormalHitMontage;

	uint8 AirNum = static_cast<uint8>(FDamageType::AIR);
	bUsingDamageTypeEffect[AirNum] = true;
	DamagedMontages[AirNum] = AirHitMontage;

	uint8 AirAttackNum = static_cast<uint8>(FDamageType::AIRATTACK);
	bUsingDamageTypeEffect[AirAttackNum] = true;
	DamagedMontages[AirAttackNum] = AirAttackHitMontage;

	uint8 StrongAttackNum = static_cast<uint8>(FDamageType::STRONGATTACK);
	bUsingDamageTypeEffect[StrongAttackNum] = true;
	DamagedMontages[StrongAttackNum] = StrongAttackHitMontage;

	uint8 PoisionNum = static_cast<uint8>(FDamageType::POISION);
	bUsingDamageTypeEffect[PoisionNum] = true;

	uint8 BurnNum = static_cast<uint8>(FDamageType::BURN);
	bUsingDamageTypeEffect[BurnNum] = true;

#pragma region Poision Material
	//@LOAD Poision Material
	{
		Path = L"Material'/Game/_Mine/Mesh/HM_Assassin/ParagonCountess/Characters/Heroes/Countess/Materials/M_Countess_Skin_Poision.M_Countess_Skin_Poision'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> PoisionMat(*Path);
		if (PoisionMat.Succeeded())
		{
			Mat_Poision_0 = PoisionMat.Object;
		}
	}
#pragma endregion

	//@LOAD Burn Particle - ParticleComp
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_BurningActor.PS_BurningActor'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> BurnPT(*Path);
		if (BurnPT.Succeeded())
		{
			BurnParticle = BurnPT.Object;
		}
	}

}

void UCHM_AssassinHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_Assassin = Cast<ACHM_Assassin>(GetOwner());
	check(HM_Assassin);

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
		Map_OriginPoisionMaterial.Add(0, HM_Assassin->GetMesh()->GetMaterial(0));
	}
}

void UCHM_AssassinHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssassinHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	IfFalseRet(bCanAttackFromOther);

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_AssassinHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate 실행.
	HM_Assassin->OnActionResetState.Broadcast(HM_Assassin);

	Type->OnHittingProcess(AttackingActor, HM_Assassin, this, DamageAmount);
}


