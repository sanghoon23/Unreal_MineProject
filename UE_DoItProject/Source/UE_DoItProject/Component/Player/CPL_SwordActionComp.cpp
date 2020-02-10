#include "CPL_SwordActionComp.h"
#include "Global.h"

#include "State/Player/Sword/CPL_SDRollAction.h"

UCPL_SwordActionComp::UCPL_SwordActionComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create Roll Action
	{
		UC_BaseActionState* RollAction = CreateDefaultSubobject<UCPL_SDRollAction>("RollAction");
		BaseActionArray.Emplace(RollAction);
	}
}

void UCPL_SwordActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SwordActionComp::BeginPlay()
{
	Super::BeginPlay();
}

/* @Type - BaseActionArray �� ���� �ִ� IBaseAction Enum. ��, Array[Type] */
IIC_BaseAction * UCPL_SwordActionComp::GetIBaseAction(uint8 Type)
{
	if (Type > BaseActionArray.Num() - 1)
	{
		CLog::Print(L"SwordActionComp GetIBaseAction Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAction>(BaseActionArray[Type]);
}
