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

	//@LOAD Chractor SkeletalMesh - (Default)
	{
		//@Origin
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Assassin/SM_Countess.SM_Countess'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> OriginSK(*Path);
		if (OriginSK.Succeeded())
		{
			OriginCharactorMesh = OriginSK.Object;
		}

		//@Poision
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Assassin/SM_Countess_ForPoision.SM_Countess_ForPoision'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> PoisionSK(*Path);
		if (PoisionSK.Succeeded())
		{
			PoisionCharactorMesh = PoisionSK.Object;
		}

		//@ForDeath
		Path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Assassin/SM_Countess_ForDeath.SM_Countess_ForDeath'";
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
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_HitCombo.HM_Assa_Mon_HitCombo'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> comboHit(*Path);
		if (comboHit.Succeeded())
			HitComboMon = comboHit.Object;
	}

	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_NormalHit_Left.HM_Assa_Mon_NormalHit_Left'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit_left(*Path);
		if (normalHit_left.Succeeded())
			NormalHitMontage_Left = normalHit_left.Object;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Assassin/Hit/HM_Assa_Mon_NormalHit_Right.HM_Assa_Mon_NormalHit_Right'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit_right(*Path);
		if (normalHit_right.Succeeded())
			NormalHitMontage_Right = normalHit_right.Object;
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
	DamagedMontages[NormalNum] = NormalHitMontage_Left;

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
	IfTrueRet(HM_Assassin->IsDeath());

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_AssassinHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//DamagedMontages[NormalNum] = NormalHitMontage_Left;

	//왼쪽 -, 오른쪽 +
	//@Left Right NormalHitMontage 여기서 처리?? -->> 계산...
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector AttackerLocation = AttackingActor->GetActorLocation();

		FVector Dir = AttackerLocation - OwnerLocation;
		Dir.Z = 0.0f;
		Dir.Normalize();

		FVector CrossVec = GetOwner()->GetActorForwardVector() ^ Dir;
		//float RidAngle = GetOwner()->GetActorRightVector() | Dir;
		//float DegAngle = FMath::RadiansToDegrees(acosf(RidAngle));

		uint8 NormalNum = static_cast<uint8>(FDamageType::NORMAL);
		if (CrossVec.Z < 0.0f) //@왼
		{
			DamagedMontages[NormalNum] = NormalHitMontage_Left;
		}
		else //@오른
		{
			DamagedMontages[NormalNum] = NormalHitMontage_Right;
		}
	}

	//@Delegate 실행.
	HM_Assassin->OnActionResetState.Broadcast(HM_Assassin);

	Type->OnHittingProcess(AttackingActor, HM_Assassin, this, DamageAmount);
}


