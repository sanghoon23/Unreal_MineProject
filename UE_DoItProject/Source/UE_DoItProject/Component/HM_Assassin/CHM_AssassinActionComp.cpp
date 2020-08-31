#include "CHM_AssassinActionComp.h"

#include "State/HM_Assassin/CHM_AssaActionAnger.h"

UCHM_AssassinActionComp::UCHM_AssassinActionComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	{
		UC_BaseActionState* RollAction = CreateDefaultSubobject<UCHM_AssaActionAnger>("Assa_AngerAction");
		ActionArray.Emplace(RollAction);
	}
}

void UCHM_AssassinActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_AssassinActionComp::BeginPlay()
{
	Super::BeginPlay();
}

/* @Type - BaseActionArray 에 속해 있는 IBaseAction Enum. 즉, Array[Type] */
IIC_BaseAction * UCHM_AssassinActionComp::GetIBaseAction(uint8 Type)
{
	if (Type > ActionArray.Num() - 1)
	{
		UE_LOG(LogTemp, Warning, L"AssassinActionComp GetIBaseAction Array Excess!!");
		return nullptr;
	}

	return Cast<IIC_BaseAction>(ActionArray[Type]);
}
