#include "CPL_SwordAttackComp.h"
#include "Global.h"

#include "State/Player/Sword/CPL_SDAttackBasic.h"

UCPL_SwordAttackComp::UCPL_SwordAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// Create State
	{
		UCPL_SwordBaseAttack* SD_BasicAttack;

		SD_BasicAttack = CreateDefaultSubobject<UCPL_SDAttackBasic>("BasicAttack");
		SD_BasicAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordStateArray.Emplace(SD_BasicAttack);
	}
	#pragma endregion
}


void UCPL_SwordAttackComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_SwordAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp 참고.
// @Type - AttackStateArray 에서 가져올 AttackType
// @Return - @Type 에 해당하는 IBaseAttack
IIC_BaseAttack*  UCPL_SwordAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTureRetResult
	(
		Type >= static_cast<uint8>(SwordAttackType::END),
		nullptr
	);

	SwordAttackType SetType = static_cast<SwordAttackType>(Type);
	AttackType = SetType;

	int CurrentType = static_cast<int>(AttackType);
	return Cast<IIC_BaseAttack>(SwordStateArray[CurrentType]);
}

// - IC_AttackComp 참고.
// @현재 설정된 AttackType 의 Interface_BaseAttack 을 가져옴.
IIC_BaseAttack * UCPL_SwordAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > SwordStateArray.Num() - 1)
	{
		CLog::Print(L"SDAttackComp IBaseAttack Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAttack>(SwordStateArray[CurrentType]);
}

