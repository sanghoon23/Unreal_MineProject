#include "CHM_ShamanAttackComp.h"
#include "Global.h"
#include "Interface/IC_Component.h"

#include "State/Base/C_BaseAttackState.h"
#include "State/HM_Shaman/CHM_ShamanFirstAttack.h"
#include "State/HM_Shaman/CHM_ShamanEnergyPa.h"

UCHM_ShamanAttackComp::UCHM_ShamanAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// @FirstCombo
	{
		UC_BaseAttackState* FirstAttack = CreateDefaultSubobject<UCHM_ShamanFirstAttack>("HM_Shaman_First");
		AttackStateArray.Emplace(FirstAttack);
	}

	// @SecondCombo
	{
		UC_BaseAttackState* SecondAttack = CreateDefaultSubobject<UCHM_ShamanEnergyPa>("HM_Shaman_Second");
		AttackStateArray.Emplace(SecondAttack);
	}
	#pragma endregion
}

void UCHM_ShamanAttackComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCHM_ShamanAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp 참고.
// @Type - AttackStateArray 에서 가져올 AttackType
// @Return - @Type 에 해당하는 IBaseAttack
IIC_BaseAttack * UCHM_ShamanAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTrueRetResult
	(
		Type >= static_cast<uint8>(HM_ShamanAttackType::END),
		nullptr
	);

	// @기존 AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @들어온 Type
	HM_ShamanAttackType SetType = static_cast<HM_ShamanAttackType>(Type);
	if (AttackType == SetType)
	{
		// @Tick true
		IIC_Component* IC_Comp = Cast<IIC_Component>(AttackStateArray[Type]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(true);
		}

		return Cast<IIC_BaseAttack>(AttackStateArray[Type]); //@return
	}
	else if (AttackType != SetType)
	{
		// @Tick false
		IIC_Component* IC_Comp = Cast<IIC_Component>(AttackStateArray[BeforeTypeNum]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(false);
		}

		// @EndAttack Call
		AttackStateArray[BeforeTypeNum]->EndAttackDeleFunc.Broadcast();
		AttackType = SetType;
	}

	uint8 AfterTypeNum = static_cast<uint8>(AttackType);

	// @Tick true
	IIC_Component* IC_Comp = Cast<IIC_Component>(AttackStateArray[AfterTypeNum]);
	if (IC_Comp != nullptr)
	{
		IC_Comp->IsRunTick(true);
	}

	return Cast<IIC_BaseAttack>(AttackStateArray[AfterTypeNum]);
}

// - IC_AttackComp 참고.
// @현재 설정된 AttackType 의 Interface_BaseAttack 을 가져옴.
IIC_BaseAttack * UCHM_ShamanAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > AttackStateArray.Num() - 1)
	{
		CLog::Print(L"MGAttackComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return Cast<IIC_BaseAttack>(AttackStateArray[CurrentType]);
}
