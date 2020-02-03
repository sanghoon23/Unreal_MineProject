
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


void UCPlayerAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

