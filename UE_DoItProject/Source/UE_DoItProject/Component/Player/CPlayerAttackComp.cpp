
#include "CPlayerAttackComp.h"

UCPlayerAttackComp::UCPlayerAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPlayerAttackComp::BeginPlay()
{
	Super::BeginPlay();

	
}

IIC_BaseAttack * UCPlayerAttackComp::GetAttackStateMember(uint8 Num)
{
	return nullptr;
}


void UCPlayerAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

