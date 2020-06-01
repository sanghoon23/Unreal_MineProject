#include "CPL_MageActionComp.h"
#include "Global.h"

#include "State/Player/Mage/CPL_MGActionTeleprot.h"
#include "State/Player/Mage/CPL_MGActionJump.h"

UCPL_MageActionComp::UCPL_MageActionComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create Roll Action
	{
		UC_BaseActionState* RollAction = CreateDefaultSubobject<UCPL_MGActionTeleprot>("MG_Teleprot");
		MG_ActionArray.Emplace(RollAction);
	}

	// Create Jump Action
	{
		UC_BaseActionState* JumpAction = CreateDefaultSubobject<UCPL_MGActionJump>("MG_JumpAction");
		MG_ActionArray.Emplace(JumpAction);
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

/* @Type - MG_ActionArray 에 속해 있는 IBaseAction Enum. 즉, Array[Type] */
IIC_BaseAction * UCPL_MageActionComp::GetIBaseAction(uint8 Type)
{
	if (Type > MG_ActionArray.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, L"MageActionComp GetIBaseAction Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAction>(MG_ActionArray[Type]);
}
