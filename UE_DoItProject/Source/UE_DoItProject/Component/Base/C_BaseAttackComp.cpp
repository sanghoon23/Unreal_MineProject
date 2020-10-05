#include "C_BaseAttackComp.h"
#include "Global.h"

UC_BaseAttackComp::UC_BaseAttackComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UC_BaseAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UC_BaseAttackComp::BeginPlay()
{
	Super::BeginPlay();

	
}
