#include "CPL_HitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "DamageType/Base/CDamageType_Base.h"

UCPL_HitComp::UCPL_HitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

	#pragma region Hit Montages
	//@Super
	{
		// 'CanHitCom' Montage
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Hit/Player_Mon_HitCombo.Player_Mon_HitCombo'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> comboHit(*Path);
		if (comboHit.Succeeded())
			HitComboMon = comboHit.Object;
	}

	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Hit/Player_Mon_NormalHit1.Player_Mon_NormalHit1'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'Air First' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Hit/Player_Mon_AirFirstHit.Player_Mon_AirFirstHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airHit(*Path);
		if (airHit.Succeeded())
			AirHitMontage = airHit.Object;
	}

	// 'AirAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Hit/Player_Mon_AirAttackHit.Player_Mon_AirAttackHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> airAttackHit(*Path);
		if (airAttackHit.Succeeded())
			AirAttackHitMontage = airAttackHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Hit/Player_Mon_StorngHit.Player_Mon_StorngHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

	// 'Stun' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Hit/Player_Mon_Stun_.Player_Mon_Stun_'";
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

void UCPL_HitComp::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(Player);
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		//bCanAttackFromOther = true;
		bDamaged = true; //@다른 몽타주가 실행되기 때문에
		bCanHitCombo = false;
	});
}

void UCPL_HitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_HitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	IfFalseRet(bCanAttackFromOther);
	IfTrueRet(Player->IsDeath());

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp OnHit - ConditionType END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//CLog::Print(L"CPL_HitComp ActionResetState Before");

	//@Delegate 실행.
	Player->OnActionResetState.Broadcast(Player);

	Type->OnHittingProcess(AttackingActor, Player, this, DamageAmount);

	////@Montage 실행 - bBlockDamageMontage 변수 여부 ( BaseHitComp )
	//IfTrueRet(bBlockDamagedMontage);

	////#Edit - 0601
	////@공격하고 있을땐 맞는 몽타주 실행하지 않음
	//IfTrueRet(Player->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking());

	////@Player 는 Combo 적용하지 않음.
	////if (bCanHitCombo == true)
	////{
	////	if (HitComboMon != nullptr)
	////	{
	////		Player->ActorAnimMonPlay(HitComboMon, 0.6f, true);
	////		SetCanHittedCombo(false); //@false
	////		return; //@return
	////	}
	////}

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
	//	Player->ActorAnimMonPlay(RunMontage, 0.6f, true);
	//}

}

