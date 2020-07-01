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
	DamagedMontages[NormalNum] = NormalHitMontage;

	uint8 AirNum = static_cast<uint8>(FDamageType::AIR);
	DamagedMontages[AirNum] = AirHitMontage;

	uint8 AirAttackNum = static_cast<uint8>(FDamageType::AIRATTACK);
	DamagedMontages[AirAttackNum] = AirAttackHitMontage;

	uint8 StrongAttackNum = static_cast<uint8>(FDamageType::STRONGATTACK);
	DamagedMontages[StrongAttackNum] = StrongAttackHitMontage;

	uint8 StunNum = static_cast<uint8>(FDamageType::STUN);
	DamagedMontages[StunNum] = StunHitMontage;

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

	// TODO : Poision Material �����
	////@LOAD Poision Material
	//{
	//	Path = L"Material'/Game/_Mine/Mesh/HM_Basic/CharM_Standard/M_Char_Standard_Poision.M_Char_Standard_Poision'";
	//	ConstructorHelpers::FObjectFinder<UMaterialInterface> PoisionMat(*Path);
	//	if (PoisionMat.Succeeded())
	//	{
	//		PoisionMaterial = PoisionMat.Object;
	//	}
	//}

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
		bDamaged = true; //@�ٸ� ��Ÿ�ְ� ����Ǳ� ������
		bCanHitCombo = false;
	});
}

void UCPL_HitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Test Code
	//bool bBeated = Super::IsBeated();
	//CLog::Print(bBeated);
}

void UCPL_HitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * Type, float DamageAmount)
{
	Super::OnHit(AttackingActor, Type, DamageAmount);
	check(AttackingActor);
	check(Type);

	IfFalseRet(bCanAttackFromOther);

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp OnHit - ConditionType END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	/////DamageType Process
	//if (IsDamagedFromOther() == true)
	//{
	//}

	//@Delegate ����.
	Player->OnActionResetState.Broadcast(Player);

	Type->OnHittingProcess(AttackingActor, Player, this, DamageAmount);

	////@Montage ���� - bBlockDamageMontage ���� ���� ( BaseHitComp )
	//IfTrueRet(bBlockDamagedMontage);

	////#Edit - 0601
	////@�����ϰ� ������ �´� ��Ÿ�� �������� ����
	//IfTrueRet(Player->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking());

	////@Player �� Combo �������� ����.
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

