#include "C_BaseActionComp.h"
#include "Global.h"

UC_BaseActionComp::UC_BaseActionComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UC_BaseActionComp::BeginPlay()
{
	Super::BeginPlay();

	
}

void UC_BaseActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*
@Type - BaseActionArray 에 속해 있는 IBaseAction Enum.
즉, Array[Type]
*/
//IIC_BaseAction * UC_BaseActionComp::GetIBaseAction(uint8 Type)
//{
//	return nullptr;
//	//if (Type > BaseActionArray.Num() - 1)
//	//{
//	//	CLog::Print(L"GetIBaseAction Array Excess!!");
//	//	return nullptr;
//	//}
//	//return Cast<IIC_BaseAction>(BaseActionArray[Type]);
//}