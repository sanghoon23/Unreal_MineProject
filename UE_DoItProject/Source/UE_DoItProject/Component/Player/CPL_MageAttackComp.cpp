#include "CPL_MageAttackComp.h"
#include "Global.h"
#include "Interface/IC_Component.h"

#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "State/Player/Mage/CPL_MGAttackBasic.h"
#include "State/Player/Mage/CPL_MGAttackMagicBall.h"
#include "State/Player/Mage/CPL_MGAttackFireRange.h"
#include "State/Player/Mage/CPL_MGAttackPoisioning.h"
#include "State/Player/Mage/CPL_MGAttackFreezing.h"

UCPL_MageAttackComp::UCPL_MageAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// @BasicAttack
	{
		UCPL_MageBaseAttack* MG_BasicAttack = CreateDefaultSubobject<UCPL_MGAttackBasic>("Mage_AttackFrist");
		MG_BasicAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));
		MageAttackStateArray.Emplace(MG_BasicAttack);
	}

	// @MagicBall
	{
		UCPL_MageBaseAttack* MG_MagicBallAttack = CreateDefaultSubobject<UCPL_MGAttackMagicBall>("Mage_AttackSecond");
		MG_MagicBallAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));
		MageAttackStateArray.Emplace(MG_MagicBallAttack);
	}

	// @FireRange
	{
		UCPL_MageBaseAttack* MG_FireRange = CreateDefaultSubobject<UCPL_MGAttackFireRange>("Mage_AttackThrid");
		MG_FireRange->SetOwnerPawn(Cast<APawn>(GetOwner()));
		MageAttackStateArray.Emplace(MG_FireRange);
	}

	// @Poisioning
	{
		UCPL_MageBaseAttack* MG_Poisioning = CreateDefaultSubobject<UCPL_MGAttackPoisioning>("Mage_AttackFour");
		MG_Poisioning->SetOwnerPawn(Cast<APawn>(GetOwner()));
		MageAttackStateArray.Emplace(MG_Poisioning);
	}

	// @Poisioning
	{
		UCPL_MageBaseAttack* MG_Freezing = CreateDefaultSubobject<UCPL_MGAttackFreezing>("Mage_AttackFive");
		MG_Freezing->SetOwnerPawn(Cast<APawn>(GetOwner()));
		MageAttackStateArray.Emplace(MG_Freezing);
	}

	#pragma endregion
}


void UCPL_MageAttackComp::BeginPlay()
{
	Super::BeginPlay();
}


void UCPL_MageAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp 참고.
// @Ref
// @Type - 바꿀 AttackType 을 넘겨줌.
// #Edit *0220
// 기존 타입과 바뀌면, EndAttack BroadCast 후, 타입 세팅, 세팅한 Type IBaseAttack 리턴.
IIC_BaseAttack * UCPL_MageAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTrueRetResult
	(
		Type >= static_cast<uint8>(EMageAttackType::END), 
		nullptr
	);

	// @기존 AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @들어온 Type
	EMageAttackType SetType = static_cast<EMageAttackType>(Type);
	if (AttackType == SetType)
	{
		// @Tick true
		IIC_Component* IC_Comp = Cast<IIC_Component>(MageAttackStateArray[Type]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(true);
		}

		return Cast<IIC_BaseAttack>(MageAttackStateArray[Type]);
	}
	else if (AttackType != SetType)
	{
		// @Tick false
		IIC_Component* IC_Comp = Cast<IIC_Component>(MageAttackStateArray[BeforeTypeNum]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(false);
		}

		// @EndAttack Call
		MageAttackStateArray[BeforeTypeNum]->EndAttackDeleFunc.Broadcast();
		AttackType = SetType;
	}

	uint8 AfterTypeNum = static_cast<uint8>(AttackType);

	// @Tick true
	IIC_Component* IC_Comp = Cast<IIC_Component>(MageAttackStateArray[AfterTypeNum]);
	if (IC_Comp != nullptr)
	{
		IC_Comp->IsRunTick(true);
	}

	return Cast<IIC_BaseAttack>(MageAttackStateArray[Type]);
}

// - IC_AttackComp 참고.
// @Type - AttackStateArray 에서 가져올 AttackType
// @Return - @Type 에 해당하는 IBaseAttack
IIC_BaseAttack * UCPL_MageAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > MageAttackStateArray.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, L"MGAttackComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}


	return Cast<IIC_BaseAttack>(MageAttackStateArray[CurrentType]);
}

