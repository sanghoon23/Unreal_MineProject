#include "CDecalActor_Targeting.h"
#include "Global.h"
#include "Components/DecalComponent.h"

ACDecalActor_Targeting::ACDecalActor_Targeting()
{
	PrimaryActorTick.bCanEverTick = true;

	FString strPath = L"";

	DecalComp = CreateDefaultSubobject<UDecalComponent>("DecalComp");
	RootComponent = DecalComp;

	DecalComp->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	strPath = L"Material'/Game/_Mine/UseMaterial/Decal/Mat_DecalRed_Targeting.Mat_DecalRed_Targeting'";
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat_Targeting(*strPath);
	if (Mat_Targeting.Succeeded())
	{
		TargetingCircle = Mat_Targeting.Object;
	}
}

void ACDecalActor_Targeting::BeginPlay()
{
	Super::BeginPlay();

	SetDecalCompMat(TargetingCircle);
}

void ACDecalActor_Targeting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDecalActor_Targeting::SetDecalCompMat(UMaterialInterface * Material)
{
	check(Material);
	DecalComp->SetMaterial(0, Material);
}

void ACDecalActor_Targeting::SetDecalCompRotation(FRotator Rotate)
{
	DecalComp->SetRelativeRotation(Rotate);
}

void ACDecalActor_Targeting::SetDecalSize(float fSize)
{
	FVector SizeVec = FVector(fSize);
	DecalComp->DecalSize = SizeVec;
}

void ACDecalActor_Targeting::SetDecalSize(FVector Size)
{
	DecalComp->DecalSize = Size;
}
