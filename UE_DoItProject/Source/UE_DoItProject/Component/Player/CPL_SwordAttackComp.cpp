#include "CPL_SwordAttackComp.h"
#include "Global.h"

#include "Charactor/Player/CPlayer.h"

#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "State/Player/Sword/CPL_SDAttackBasic.h"
#include "State/Player/Sword/CPL_SDAttackUpper.h"
#include "State/Player/Sword/CPL_SDAttackFinish.h"
#include "State/Player/Sword/CPL_SDAttackWindmilSlash.h"
#include "State/Player/Sword/CPL_SDAttackBackRange.h"

UCPL_SwordAttackComp::UCPL_SwordAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// Create First Attack ( Basic Attack )
	{
		UCPL_SwordBaseAttack* SD_BasicAttack = nullptr;

		SD_BasicAttack = CreateDefaultSubobject<UCPL_SDAttackBasic>("Sword_AttackFirst");
		SD_BasicAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_BasicAttack);
	}

	// Create Second Attack ( Upper Attack )
	{
		UCPL_SwordBaseAttack* SD_UpperAttack = nullptr;

		SD_UpperAttack = CreateDefaultSubobject<UCPL_SDAttackUpper>("Sword_AttackSecond");
		SD_UpperAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_UpperAttack);
	}

	// Create Thrid Attack ( Finish Attack )
	{
		UCPL_SwordBaseAttack* SD_FinishAttack = nullptr;

		SD_FinishAttack = CreateDefaultSubobject<UCPL_SDAttackFinish>("Sword_AttackThird");
		SD_FinishAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_FinishAttack);
	}

	// Create Four Attack ( WindmilSlash )
	{
		UCPL_SwordBaseAttack* SD_WindmilSlash = nullptr;

		SD_WindmilSlash = CreateDefaultSubobject<UCPL_SDAttackWindmilSlash>("Sword_AttackFour");
		SD_WindmilSlash->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_WindmilSlash);
	}

	// Create Five Attack ( BackRange )
	{
		UCPL_SwordBaseAttack* SD_BackRange = nullptr;

		SD_BackRange = CreateDefaultSubobject<UCPL_SDAttackBackRange>("Sword_AttackFive");
		SD_BackRange->SetOwnerPawn(Cast<APawn>(GetOwner()));

		SwordAttackStateArray.Emplace(SD_BackRange);
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
// #Edit *0220
// 기존 타입과 바뀌면, EndAttack BroadCast 후, 타입 세팅, 세팅한 Type IBaseAttack 리턴.
IIC_BaseAttack*  UCPL_SwordAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTureRetResult
	(
		Type >= static_cast<uint8>(ESwordAttackType::END),
		nullptr
	);

	// @기존 AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @들어온 Type
	ESwordAttackType SetType = static_cast<ESwordAttackType>(Type);
	if (AttackType == SetType)
	{
		return Cast<IIC_BaseAttack>(SwordAttackStateArray[Type]);
	}
	else if (AttackType != SetType) // 바뀌었다면
	{
		// @EndAttack Call
		SwordAttackStateArray[BeforeTypeNum]->EndAttackDeleFunc.Broadcast();
		AttackType = SetType;
	}

	uint8 AfterTypeNum = static_cast<uint8>(AttackType);
	return Cast<IIC_BaseAttack>(SwordAttackStateArray[AfterTypeNum]);
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

