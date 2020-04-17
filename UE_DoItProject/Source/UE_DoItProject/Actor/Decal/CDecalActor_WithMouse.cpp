#include "CDecalActor_WithMouse.h"
#include "Global.h"
#include "Components/DecalComponent.h"

ACDecalActor_WithMouse::ACDecalActor_WithMouse()
{
	PrimaryActorTick.bCanEverTick = true;

	FString strPath = L"";

	DecalComp = CreateDefaultSubobject<UDecalComponent>("DecalComp");
	RootComponent = DecalComp;

	DecalComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void ACDecalActor_WithMouse::BeginPlay()
{
	Super::BeginPlay();
}

void ACDecalActor_WithMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDecalActor_WithMouse::SetDecalCompMat(UMaterialInterface * Material)
{
	check(Material);
	DecalComp->SetMaterial(0, Material);
}

void ACDecalActor_WithMouse::SetDecalCompRotation(FRotator Rotate)
{
	DecalComp->SetRelativeRotation(Rotate);
}

void ACDecalActor_WithMouse::SetDecalSize(FVector Size)
{
	DecalComp->DecalSize = Size;
}
