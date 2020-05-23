#include "CPL_SwordActionComp.h"
#include "Global.h"

#include "State/Player/Sword/CPL_SDActionRoll.h"
#include "State/Player/Sword/CPL_SDActionJump.h"

UCPL_SwordActionComp::UCPL_SwordActionComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Create Roll Action
	{
		UC_BaseActionState* RollAction = CreateDefaultSubobject<UCPL_SDActionRoll>("SD_RollAction");
		RollAction->SetOwnerPawn(Cast<APawn>(GetOwner()));
		SD_ActionArray.Emplace(RollAction);
	}

	// Create Jump Action
	{
		UC_BaseActionState* JumpAction = CreateDefaultSubobject<UCPL_SDActionJump>("SD_JumpAction");
		JumpAction->SetOwnerPawn(Cast<APawn>(GetOwner()));
		SD_ActionArray.Emplace(JumpAction);
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

/* @Type - BaseActionArray 에 속해 있는 IBaseAction Enum. 즉, Array[Type] */
IIC_BaseAction * UCPL_SwordActionComp::GetIBaseAction(uint8 Type)
{
	if (Type > SD_ActionArray.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, L"SwordActionComp GetIBaseAction Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAction>(SD_ActionArray[Type]);
}
