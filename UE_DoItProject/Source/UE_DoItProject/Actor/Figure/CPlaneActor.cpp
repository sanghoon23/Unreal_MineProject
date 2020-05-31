#include "CPlaneActor.h"
#include "Global.h"

ACPlaneActor::ACPlaneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = SceneComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(SceneComp);

	FString strPath = L"";

	strPath = L"StaticMesh'/Engine/BasicShapes/Plane.Plane'";
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Plane(*strPath);
	if (SM_Plane.Succeeded())
	{
		StaticMesh->SetStaticMesh(SM_Plane.Object);
	}

	//@Setting - 기본적으로 NoCollision 으로 가정
	StaticMesh->SetGenerateOverlapEvents(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -45.0f));
}

void ACPlaneActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlaneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPlaneActor::SetMaterial(UMaterialInterface * Material)
{
	check(Material);
	StaticMesh->SetMaterial(0, Material);
}

void ACPlaneActor::SetSMVisibility(bool bValue)
{
	StaticMesh->SetVisibility(bValue);
}
