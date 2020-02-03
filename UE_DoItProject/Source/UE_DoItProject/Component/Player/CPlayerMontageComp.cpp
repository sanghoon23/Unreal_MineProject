
#include "CPlayerMontageComp.h"

UCPlayerMontageComp::UCPlayerMontageComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPlayerMontageComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPlayerMontageComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

