#include "CPL_SwordAttackComp.h"
#include "Global.h"

#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "State/Player/Sword/CPL_SDAttackBasic.h"
#include "State/Player/Sword/CPL_SDAttackUpper.h"

UCPL_SwordAttackComp::UCPL_SwordAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// Create First Attack (Basic Attack)
	{
		UCPL_SwordBaseAttack* SD_BasicAttack = nullptr;

		SD_BasicAttack = CreateDefaultSubobject<UCPL_SDAttackBasic>("Sword_AttackFirst");
		SD_BasicAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_BasicAttack);
	}

	// Create Second Attack (Upper Attack)
	{
		UCPL_SwordBaseAttack* SD_UpperAttack = nullptr;

		SD_UpperAttack = CreateDefaultSubobject<UCPL_SDAttackUpper>("Sword_AttackSecond");
		SD_UpperAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_UpperAttack);
	}

	#pragma endregion
}


void UCPL_SwordAttackComp::BeginPlay()
{
	Super::BeginPlay();
	
	//// @SetOwnerPawn - IBaseAttack
	//for (UCPL_SwordBaseAttack* BaseAttack : SwordStateArray)
	//{
	//	if (BaseAttack == nullptr) continue;
	//	BaseAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));
	//}
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

	return Cast<IIC_BaseAttack>(SwordAttackStateArray[Type]);
}

// - IC_AttackComp 참고.
// @현재 설정된 AttackType 의 Interface_BaseAttack 을 가져옴.
IIC_BaseAttack * UCPL_SwordAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > SwordAttackStateArray.Num() - 1)
	{
		CLog::Print(L"SDAttackComp IBaseAttack Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAttack>(SwordAttackStateArray[CurrentType]);
}

