#include "CPL_BlendCameraActor.h"
#include "Global.h"

ACPL_BlendCameraActor::ACPL_BlendCameraActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACPL_BlendCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACPL_BlendCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
