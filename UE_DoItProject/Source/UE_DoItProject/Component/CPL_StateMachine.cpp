#include "CPL_StateMachine.h"
#include "Global.h"

#include "Charactor/Player/CPlayer.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_EquipComp.h"
#include "Component/Player/CPL_MageAttackComp.h"
#include "Component/Player/CPL_SwordAttackComp.h"

#include "Item/CPL_Sword.h"

UCPL_StateMachine::UCPL_StateMachine()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Create AttackComp
	{
		UC_BaseAttackComp* MageAttack = CreateDefaultSubobject<UCPL_MageAttackComp>("MageAttack");
		UC_BaseAttackComp* SwordAttack = CreateDefaultSubobject<UCPL_MageAttackComp>("SwordAttack");

		AttackCompArray.Emplace(MageAttack);
		AttackCompArray.Emplace(SwordAttack);
	}
}


void UCPL_StateMachine::BeginPlay()
{
	Super::BeginPlay();
}


void UCPL_StateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

IIC_AttackComp * UCPL_StateMachine::GetIAttackComp()
{
	int Type = static_cast<int>(CurrentStateType);
	return Cast<IIC_AttackComp>(AttackCompArray[Type]);
}

void UCPL_StateMachine::OnSwapState()
{
	// @State Swap 'Àü'
	// @Mage State
	if (CurrentStateType == PlayerStateType::MAGE)
	{
		UnMageState.Broadcast();
	}
	// @Sword State
	else if (CurrentStateType == PlayerStateType::SWORD)
	{
		UnSwordState.Broadcast();
	}

	// @State Swap 'Áß'
	int Type = static_cast<int>(CurrentStateType);
	++Type;
	(Type == static_cast<int>(PlayerStateType::END))
		? CurrentStateType = static_cast<PlayerStateType>(0)
		: CurrentStateType = static_cast<PlayerStateType>(Type);

	// @State Swap 'ÈÄ'
	// @Mage State
	if (CurrentStateType == PlayerStateType::MAGE)
	{
		OnMageState.Broadcast();
	}
	// @Sword State
	else if (CurrentStateType == PlayerStateType::SWORD)
	{
		OnSwordState.Broadcast();
	}
}

