#include "CHM_BasicAttackComp.h"
#include "Global.h"

#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"
#include "State/HM_Basic/CHM_BasicFirstCombo.h"

UCHM_BasicAttackComp::UCHM_BasicAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// Create State
	{
		UCHM_BasicBaseAttack* FirstCombo = CreateDefaultSubobject<UCHM_BasicFirstCombo>("HM_Basic_FirstCombo");
		FirstCombo->SetOwnerPawn(Cast<APawn>(GetOwner()));

		BasicAttackStateArray.Add(FirstCombo);
	}
	#pragma endregion
}

void UCHM_BasicAttackComp::BeginPlay()
{
	Super::BeginPlay();

	//// @SetOwnerPawn - IBaseAttack
	//for (UCHM_BasicBaseAttack* BaseAttack : AttackStateArray)
	//{
	//	if (BaseAttack == nullptr) continue;
	//	BaseAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));
	//}
}

void UCHM_BasicAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp 참고.
// @Type - AttackStateArray 에서 가져올 AttackType
// @Return - @Type 에 해당하는 IBaseAttack
IIC_BaseAttack * UCHM_BasicAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTureRetResult
	(
		Type >= static_cast<uint8>(HM_BasicAttackType::END),
		nullptr
	);

	// @SetType
	HM_BasicAttackType SetType = static_cast<HM_BasicAttackType>(Type);
	AttackType = SetType;

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[Type]);
}

// - IC_AttackComp 참고.
// @현재 설정된 AttackType 의 Interface_BaseAttack 을 가져옴.
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

