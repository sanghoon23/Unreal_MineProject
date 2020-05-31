#include "CPL_HitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "DamageType/Base/CDamageType_Base.h"

UCPL_HitComp::UCPL_HitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

	#pragma region Hit Montages
	// 'Normal' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Damaged/Player_Mon_NormalAttack.Player_Mon_NormalAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> normalHit(*Path);
		if (normalHit.Succeeded())
			NormalHitMontage = normalHit.Object;
	}

	// 'StrongAttack' Hit Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Damaged/Player_Mon_StorngAttack.Player_Mon_StorngAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> strongAttackHit(*Path);
		if (strongAttackHit.Succeeded())
			StrongAttackHitMontage = strongAttackHit.Object;
	}

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
		Player->OnGravity(); //@중력키기
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

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_BasicHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}

	//@Delegate 실행.
	Player->OnActionResetState.Broadcast(Player);

	///DamageType Process
	Type->OnHittingProcess(AttackingActor, Player, this, DamageAmount);

	//@Montage 실행 - bBlockDamageMontage 변수 여부 ( BaseHitComp )
	IfTrueRet(bBlockDamagedMontage);

	const uint8 MontageNum = static_cast<uint8>(Type->GetConditionType());
	if (MontageNum >= DamagedMontages.Num()) return;
	UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
	if (RunMontage != nullptr)
	{
		Player->ActorAnimMonPlay(RunMontage, 0.6f, true);
	}

}
