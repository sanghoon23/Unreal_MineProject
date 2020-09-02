#include "CHM_MaoHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_PengMao.h"
#include "DamageType/Base/CDamageType_Base.h"

UCHM_MaoHitComp::UCHM_MaoHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

	//@LOAD Chractor SkeletalMesh - (Default)
	{
		//@Origin
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_PengMAo/FengMao_GDC.FengMao_GDC'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> OriginSK(*Path);
		if (OriginSK.Succeeded())
		{
			OriginCharactorMesh = OriginSK.Object;
		}

		//@ForDeath
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_PengMAo/FengMao_GDC_ForDeath.FengMao_GDC_ForDeath'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> ForDeathSK(*Path);
		if (ForDeathSK.Succeeded())
		{
			ForDeathCharactorMesh = ForDeathSK.Object;
		}
	}

#pragma region Hit Montages
	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_PengMao/Hit/HM_PengMao_Mon_NormalHit.HM_PengMao_Mon_NormalHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}
#pragma endregion

	//@Set Montage
	uint8 NormalNum = static_cast<uint8>(FDamageType::NORMAL);
	bUsingDamageTypeEffect[NormalNum] = true;
	DamagedMontages[NormalNum] = NormalHitMontage;

	//uint8 PoisionNum = static_cast<uint8>(FDamageType::POISION);
	//bUsingDamageTypeEffect[PoisionNum] = true;

	uint8 BurnNum = static_cast<uint8>(FDamageType::BURN);
	bUsingDamageTypeEffect[BurnNum] = true;

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

void UCHM_MaoHitComp::BeginPlay()
{
	Super::BeginPlay();

	HM_PengMao = Cast<ACHM_PengMao>(GetOwner());
	check(HM_PengMao);

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

		const uint8 ForDeathNum = static_cast<uint8>(ECharactorMeshSort::FORDEATH);
		CharactorMeshArray[ForDeathNum] = ForDeathCharactorMesh;
	}
}

void UCHM_MaoHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_MaoHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	IfFalseRet(bCanAttackFromOther);

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_PengMaoHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate 실행.
	HM_PengMao->OnActionResetState.Broadcast(HM_PengMao);

	Type->OnHittingProcess(AttackingActor, HM_PengMao, this, DamageAmount);
}

