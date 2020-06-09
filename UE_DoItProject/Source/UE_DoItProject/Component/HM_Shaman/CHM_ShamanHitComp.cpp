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
		HM_Shaman->OnGravity(); //@중력키기
	});
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

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_ShamanHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}


	//@Delegate 실행.
	HM_Shaman->OnActionResetState.Broadcast(HM_Shaman);

	///DamageType Process
	Type->OnHittingProcess(AttackingActor, HM_Shaman, this, DamageAmount);

	//#Edit 0510 - 
	//@Death Animation 은 AnimInstance 의 LocoMotion 을 이용

	//@Montage 실행 - bBlockDamageMontage 변수 여부 ( BaseHitComp )
	IfTrueRet(bBlockDamagedMontage);

	const uint8 MontageNum = static_cast<uint8>(Type->GetConditionType());
	if (MontageNum >= DamagedMontages.Num()) return;
	UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
	if (RunMontage != nullptr)
	{
		HM_Shaman->ActorAnimMonPlay(RunMontage, 0.6f, true);
	}

}
