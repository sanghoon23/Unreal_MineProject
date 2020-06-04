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
	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Damaged/Player_Mon_NormalHit.Player_Mon_NormalHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Damaged/Player_Mon_StorngHit.Player_Mon_StorngHit'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

	#pragma endregion

	#pragma region Set MontagesArray - (BaseHitComp)
	//@Set Montage
	//...

	uint8 NormalNum = static_cast<uint8>(FDamageType::NORMAL);
	DamagedMontages[NormalNum] = NormalHitMontage;

	uint8 StrongAttackNum = static_cast<uint8>(FDamageType::STRONGATTACK);
	DamagedMontages[StrongAttackNum] = StrongAttackHitMontage;

	#pragma endregion


}

void UCPL_HitComp::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		Player->OnGravity(); //@�߷�Ű��
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

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate ����.
	Player->OnActionResetState.Broadcast(Player);

	///DamageType Process
	Type->OnHittingProcess(AttackingActor, Player, this, DamageAmount);

	//@Montage ���� - bBlockDamageMontage ���� ���� ( BaseHitComp )
	IfTrueRet(bBlockDamagedMontage);

	//#Edit - 0601
	//@�����ϰ� ������ �´� ��Ÿ�� �������� ����
	IfTrueRet(Player->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking());

	const uint8 MontageNum = static_cast<uint8>(Type->GetConditionType());
	if (MontageNum >= DamagedMontages.Num()) return;
	UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
	if (RunMontage != nullptr)
	{
		Player->ActorAnimMonPlay(RunMontage, 0.6f, true);
	}

}
