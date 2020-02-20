#include "C_BaseActionState.h"
#include "Global.h"

UC_BaseActionState::UC_BaseActionState()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UC_BaseActionState::BeginPlay()
{
	Super::BeginPlay();

	
}


void UC_BaseActionState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

