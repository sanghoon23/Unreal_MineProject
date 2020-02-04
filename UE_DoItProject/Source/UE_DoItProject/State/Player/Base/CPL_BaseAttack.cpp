
#include "CPL_BaseAttack.h"

UCPL_BaseAttack::UCPL_BaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_BaseAttack::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_BaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

