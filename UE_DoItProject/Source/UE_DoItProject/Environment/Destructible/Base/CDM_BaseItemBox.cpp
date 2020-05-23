#include "CDM_BaseItemBox.h"
#include "Global.h"
#include "DestructibleActor.h"
#include "DestructibleComponent.h"

ACDM_BaseItemBox::ACDM_BaseItemBox()
{
	PrimaryActorTick.bCanEverTick = true;

	//Super Setting
	{
		HitAfterLifeTime = 2.5f;

		GetDestructibleComponent()->SetGenerateOverlapEvents(true);
	}
}

void ACDM_BaseItemBox::BeginPlay()
{
	Super::BeginPlay();

	GetDestructibleComponent()->OnComponentFracture.AddDynamic(this, &ACDM_BaseItemBox::OnFracture);
}

void ACDM_BaseItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDM_BaseItemBox::OnFracture(const FVector & HitPoint, const FVector & HitDirection)
{
	GetDestructibleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACDM_BaseItemBox::Death, HitAfterLifeTime);
}

void ACDM_BaseItemBox::SpawnItem()
{
}
