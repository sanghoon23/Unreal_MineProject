#include "CPL_MageActionComp.h"
#include "Global.h"

#include "State/Player/Mage/CPL_MGActionRoll.h"
#include "State/Player/Mage/CPL_MGActionJump.h"

UCPL_MageActionComp::UCPL_MageActionComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create Roll Action
	{
		UC_BaseActionState* RollAction = CreateDefaultSubobject<UCPL_MGActionRoll>("RollAction");
		RollAction->SetOwnerPawn(Cast<APawn>(GetOwner()));
		BaseActionArray.Emplace(RollAction);
	}

	// Create Jump Action
	{
		UC_BaseActionState* JumpAction = CreateDefaultSubobject<UCPL_MGActionJump>("JumpAction");
		JumpAction->SetOwnerPawn(Cast<APawn>(GetOwner()));
		BaseActionArray.Emplace(JumpAction);
	}
}

void UCPL_MageActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MageActionComp::BeginPlay()
{
	Super::BeginPlay();
}

/* @Type - BaseActionArray 에 속해 있는 IBaseAction Enum. 즉, Array[Type] */
IIC_BaseAction * UCPL_MageActionComp::GetIBaseAction(uint8 Type)
{
	if (Type > BaseActionArray.Num() - 1)
	{
		CLog::Print(L"MageActionComp GetIBaseAction Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAction>(BaseActionArray[Type]);
}
