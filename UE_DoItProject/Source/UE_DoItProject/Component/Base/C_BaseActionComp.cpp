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
