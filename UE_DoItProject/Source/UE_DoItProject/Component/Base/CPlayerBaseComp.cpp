#include "CPlayerBaseComp.h"
#include "Global.h"

UCPlayerBaseComp::UCPlayerBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPlayerBaseComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPlayerBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

