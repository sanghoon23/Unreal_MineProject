#include "CHM_BasicAttackComp.h"
#include "Global.h"
#include "Interface/IC_Component.h"

#include "State/Base/C_BaseAttackState.h"
#include "State/HM_Basic/CHM_BasicFirstCombo.h"

UCHM_BasicAttackComp::UCHM_BasicAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// @FirstCombo
	{
		UC_BaseAttackState* FirstAttack = CreateDefaultSubobject<UCHM_BasicFirstCombo>("HMBasic_FirstCombo");
		BasicAttackStateArray.Emplace(FirstAttack);
	}
	#pragma endregion
}

void UCHM_BasicAttackComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCHM_BasicAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp ����.
// @Type - AttackStateArray ���� ������ AttackType
// @Return - @Type �� �ش��ϴ� IBaseAttack
IIC_BaseAttack * UCHM_BasicAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTrueRetResult
	(
		Type >= static_cast<uint8>(EHM_BasicAttackType::END),
		nullptr
	);

	// @���� AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @���� Type
	EHM_BasicAttackType SetType = static_cast<EHM_BasicAttackType>(Type);
	if (AttackType == SetType)
	{
		// @Tick true
		IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[Type]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(true);
		}

		return Cast<IIC_BaseAttack>(BasicAttackStateArray[Type]); //@return
	}
	else if (AttackType != SetType)
	{
		// @Tick false
		IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[BeforeTypeNum]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(false);
		}

		// @EndAttack Call
		BasicAttackStateArray[BeforeTypeNum]->EndAttackDeleFunc.Broadcast();

		AttackType = SetType;
	}

	uint8 AfterTypeNum = static_cast<uint8>(AttackType);

	// @Tick true
	IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[AfterTypeNum]);
	if (IC_Comp != nullptr)
	{
		IC_Comp->IsRunTick(true);
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[AfterTypeNum]);
}

// - IC_AttackComp ����.
// @���� ������ AttackType �� Interface_BaseAttack �� ������.
IIC_BaseAttack * UCHM_BasicAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > BasicAttackStateArray.Num() - 1)
	{
		CLog::Print(L"MGAttackComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[CurrentType]);
}

