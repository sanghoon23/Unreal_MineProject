#include "CHM_ShamanAttackComp.h"
#include "Global.h"

#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"

UCHM_ShamanAttackComp::UCHM_ShamanAttackComp()
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
		Type >= static_cast<uint8>(HM_BasicAttackType::END),
		nullptr
	);

	// @SetType
	HM_ShamanAttackType SetType = static_cast<HM_ShamanAttackType>(Type);
	AttackType = SetType;

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[Type]);
}

// - IC_AttackComp 참고.
// @현재 설정된 AttackType 의 Interface_BaseAttack 을 가져옴.
IIC_BaseAttack * UCHM_ShamanAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > BasicAttackStateArray.Num() - 1)
	{
		CLog::Print(L"MGAttackComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[CurrentType]);
}
