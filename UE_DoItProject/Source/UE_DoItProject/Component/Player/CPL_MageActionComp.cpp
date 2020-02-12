#include "CPL_MageActionComp.h"
#include "Global.h"

#include "State/Player/Mage/CPL_MGActionRoll.h"

UCPL_MageActionComp::UCPL_MageActionComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create Roll Action
	{
		UC_BaseActionState* RollAction = CreateDefaultSubobject<UCPL_MGActionRoll>("RollAction");
		BaseActionArray.Emplace(RollAction);
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

/* @Type - BaseActionArray �� ���� �ִ� IBaseAction Enum. ��, Array[Type] */
IIC_BaseAction * UCPL_MageActionComp::GetIBaseAction(uint8 Type)
{
	if (Type > BaseActionArray.Num() - 1)
	{
		CLog::Print(L"MageActionComp GetIBaseAction Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAction>(BaseActionArray[Type]);
}
