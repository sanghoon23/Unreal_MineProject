#include "CHM_MaoAttackComp.h"
#include "Global.h"

#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"

UCHM_MaoAttackComp::UCHM_MaoAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// @FirstCombo
	{
		//UCHM_BasicBaseAttack* FirstAttack = CreateDefaultSubobject<UCHM_BasicFirstCombo>("HMBasic_FirstCombo");
		//BasicAttackStateArray.Emplace(FirstAttack);
	}
	#pragma endregion
}

void UCHM_MaoAttackComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCHM_MaoAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp ����.
// @Type - AttackStateArray ���� ������ AttackType
// @Return - @Type �� �ش��ϴ� IBaseAttack
IIC_BaseAttack * UCHM_MaoAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTrueRetResult
	(
		Type >= static_cast<uint8>(HM_PengMaoAttackType::END),
		nullptr
	);

	// @SetType
	HM_PengMaoAttackType SetType = static_cast<HM_PengMaoAttackType>(Type);
	AttackType = SetType;

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[Type]);
}

// - IC_AttackComp ����.
// @���� ������ AttackType �� Interface_BaseAttack �� ������.
IIC_BaseAttack * UCHM_MaoAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > BasicAttackStateArray.Num() - 1)
	{
		CLog::Print(L"MGAttackComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[CurrentType]);
}

