#include "CAnimInst_MediumWood.h"
#include "Global.h"

UCAnimInst_MediumWood::UCAnimInst_MediumWood()
{
}

void UCAnimInst_MediumWood::NativeBeginPlay()
{
	MediumWood = Cast<ACItemBox_MediumWood>(GetOwningActor());
	IfNullRet(MediumWood);
}

void UCAnimInst_MediumWood::NativeUpdateAnimation(float DeltaSeconds)
{
	IfNullRet(MediumWood);
	bBoxOpen = MediumWood->GetBoxOpening();
}
