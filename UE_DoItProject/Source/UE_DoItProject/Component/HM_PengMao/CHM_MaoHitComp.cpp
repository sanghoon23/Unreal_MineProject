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
		HM_PengMao->OnGravity(); //@중력키기
	});
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

	if (Type->GetConditionType() == FDamageType::END)
	{
		UE_LOG(LogTemp, Warning, L"HM_PengMaoHitComp OnHit - ConditionTtpe END!!");
		verify(Type->GetConditionType() == FDamageType::END);
	}


	//@Delegate 실행.
	HM_PengMao->OnActionResetState.Broadcast(HM_PengMao);

	///DamageType Process
	Type->OnHittingProcess(AttackingActor, HM_PengMao, this, DamageAmount);

	//#Edit 0510 - 
	//@Death Animation 은 AnimInstance 의 LocoMotion 을 이용

	//@Montage 실행 - bBlockDamageMontage 변수 여부 ( BaseHitComp )
	IfTrueRet(bBlockDamagedMontage);

	//@콤보가 가능한지,
	if (bCanHitCombo == true)
	{
		if (HitComboMon != nullptr)
		{
			HM_PengMao->ActorAnimMonPlay(HitComboMon, 0.6f, true);
			SetCanHittedCombo(false); //@false
			return; //@return
		}
	}

	//@else
	const uint8 MontageNum = static_cast<uint8>(Type->GetConditionType());
	if (MontageNum >= DamagedMontages.Num()) return;
	UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
	if (RunMontage != nullptr)
	{
		HM_PengMao->ActorAnimMonPlay(RunMontage, 0.6f, true);
	}

}

