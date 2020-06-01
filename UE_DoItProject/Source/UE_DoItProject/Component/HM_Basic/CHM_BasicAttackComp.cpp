#include "CHM_BasicAttackComp.h"
#include "Global.h"

#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"
#include "State/HM_Basic/CHM_BasicFirstCombo.h"

UCHM_BasicAttackComp::UCHM_BasicAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// @FirstCombo
	{
		UCHM_BasicBaseAttack* FirstAttack = CreateDefaultSubobject<UCHM_BasicFirstCombo>("HMBasic_FirstCombo");
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
		Type >= static_cast<uint8>(HM_BasicAttackType::END),
		nullptr
	);

	// @SetType
	HM_BasicAttackType SetType = static_cast<HM_BasicAttackType>(Type);
	AttackType = SetType;

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[Type]);
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

