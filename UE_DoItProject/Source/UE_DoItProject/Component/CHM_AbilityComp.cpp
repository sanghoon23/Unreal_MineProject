#include "CHM_AbilityComp.h"

UCHM_AbilityComp::UCHM_AbilityComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCHM_AbilityComp::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCHM_AbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

