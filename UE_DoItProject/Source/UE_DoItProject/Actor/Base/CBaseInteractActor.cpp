#include "CBaseInteractActor.h"
#include "Global.h"

ACBaseInteractActor::ACBaseInteractActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// @Create Component - BaseInteractActor
	{
		CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
		RootComponent = CollisionBox;

		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
		StaticMesh->SetupAttachment(CollisionBox);
	}

	// Setting
	{
		CollisionBox->SetGenerateOverlapEvents(true);
		CollisionBox->SetCollisionProfileName("OverlapOnlyPawn");

		StaticMesh->SetCollisionProfileName("InteractActor");
	}
}

void ACBaseInteractActor::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACBaseInteractActor::OnBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACBaseInteractActor::OnEnd);
}

void ACBaseInteractActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACBaseInteractActor::OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;
	if (GetOwner() == OtherActor) return;

	bCollisioning = true;
}

void ACBaseInteractActor::OnEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;
	if (GetOwner() == OtherActor) return;

	bCollisioning = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// *Get, *Set

UStaticMeshComponent * ACBaseInteractActor::GetStaticMeshComponent()
{
	return StaticMesh;
}
