#include "CHM_BasicEquipComp.h"
#include "Global.h"

UCHM_BasicEquipComp::UCHM_BasicEquipComp()
{
	// @FALSE
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UCHM_BasicEquipComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCHM_BasicEquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACDisplayItem * UCHM_BasicEquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

