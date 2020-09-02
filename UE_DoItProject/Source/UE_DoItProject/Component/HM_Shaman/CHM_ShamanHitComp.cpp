#include "CHM_ShamanHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Shaman.h"
#include "DamageType/Base/CDamageType_Base.h"

UCHM_ShamanHitComp::UCHM_ShamanHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

	//@LOAD Chractor SkeletalMesh - (Default)
	{
		//@Origin
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Shaman/Morigesh.Morigesh'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> OriginSK(*Path);
		if (OriginSK.Succeeded())
		{
			OriginCharactorMesh = OriginSK.Object;
		}

		//@Poision
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Shaman/Morigesh_ForPoision.Morigesh_ForPoision'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> PoisionSK(*Path);
		if (PoisionSK.Succeeded())
		{
			PoisionCharactorMesh = PoisionSK.Object;
		}

		//@ForDeath
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Shaman/Morigesh_ForDeath.Morigesh_ForDeath'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> ForDeathSK(*Path);
		if (ForDeathSK.Succeeded())
		{
			ForDeathCharactorMesh = ForDeathSK.Object;
		}
	}

#pragma region Hit Montages
	//@Super
	{
		// 'CanHitCom' Montage
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Hit/HM_ShamanMon_HitCombo.HM_ShamanMon_HitCombo'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> comboHit(*Path);
		if (comboHit.Succeeded())
			HitComboMon = comboHit.Object;
	}

	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Hit/HM_ShamanMon_NormalHit1.HM_ShamanMon_NormalHit1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'AirHitFirst' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Hit/HM_ShamanMon_AirHitFirst.HM_ShamanMon_AirHitFirst'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airHit(*Path);
		if (airHit.Succeeded())
			AirHitMontage = airHit.Object;
	}

	// 'AirAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Hit/HM_ShamanMon_AirAttackHit.HM_ShamanMon_AirAttackHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airAttackHit(*Path);
		if (airAttackHit.Succeeded())
			AirAttackHitMontage = airAttackHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Hit/HM_ShamanMon_StrongAttack.HM_ShamanMon_StrongAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

	// 'Stun' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Hit/HM_ShamanMon_Stun_.HM_ShamanMon_Stun_'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> stunHit(*Path);
		if (stunHit.Succeeded())
			StunHitMontage = stunHit.Object;
	}

#pragma endregion

#pragma region Set MontagesArray - (BaseHitComp)
	//@Set Montage
	//...

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

	uint8 StunNum = static_cast<uint8>(FDamageType::STUN);
	bUsingDamageTypeEffect[StunNum] = true;
	DamagedMontages[StunNum] = StunHitMontage;

	uint8 PoisionNum = static_cast<uint8>(FDamageType::POISION);
	bUsingDamageTypeEffect[PoisionNum] = true;

	uint8 BurnNum = static_cast<uint8>(FDamageType::BURN);
	bUsingDamageTypeEffect[BurnNum] = true;

	uint8 FreezeNum = static_cast<uint8>(FDamageType::FREEZE);
	bUsingDamageTypeEffect[FreezeNum] = true;

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

void UCHM_ShamanHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_Shaman = Cast<ACHM_Shaman>(GetOwner());
	check(HM_Shaman);

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bDamaged = true; //@다른 몽타주가 실행되기 때문에
	});

	//@Set Charactor Mesh
	{
		const uint8 OriginNum = static_cast<uint8>(ECharactorMeshSort::ORIGIN);
		CharactorMeshArray[OriginNum] = OriginCharactorMesh;

		const uint8 PoisionNum = static_cast<uint8>(ECharactorMeshSort::POISION);
		CharactorMeshArray[PoisionNum] = PoisionCharactorMesh;

		const uint8 ForDeathNum = static_cast<uint8>(ECharactorMeshSort::FORDEATH);
		CharactorMeshArray[ForDeathNum] = ForDeathCharactorMesh;
	}
}

void UCHM_ShamanHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_ShamanHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	IfFalseRet(bCanAttackFromOther);

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_ShamanHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate 실행.
	HM_Shaman->OnActionResetState.Broadcast(HM_Shaman);

	Type->OnHittingProcess(AttackingActor, HM_Shaman, this, DamageAmount);
}
