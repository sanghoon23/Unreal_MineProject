#include "CDisplayItem.h"
#include "Global.h"
#include "GameFramework/Character.h"

ACDisplayItem::ACDisplayItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create
	{
		//Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
		//RootComponent = Capsule;

		//StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
		//StaticMesh->SetupAttachment(Capsule);
	}

	// Setting
	{
		//Capsule->SetSimulatePhysics(false);
		//Capsule->SetEnableGravity(false);
		//Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//StaticMesh->SetSimulatePhysics(false);
		//StaticMesh->SetEnableGravity(false);
		//StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//StaticMesh->SetCollisionObjectType(ECollisionChannel::
	}
}

void ACDisplayItem::BeginPlay()
{
	Super::BeginPlay();
}

void ACDisplayItem::Death()
{
	Destroy();
}

void ACDisplayItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

