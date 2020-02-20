#include "CPL_TargetingSystem.h"
#include "Global.h"

#include "Charactor/Monster/Base/CHumanoidMonster.h"

UCPL_TargetingSystem::UCPL_TargetingSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_TargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_TargetingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* TargetingSystem TarceChannel 을 이용해 Target 선별하기. */
void UCPL_TargetingSystem::OnFindTargets()
{
	FVector Center = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(FindDistance);
	FCollisionQueryParams params(NAME_None, false, GetOwner());

	TArray<FOverlapResult> overlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel
	(
		overlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, //@TragetSystem TraceChannel
		Sphere,
		params
	);

	float DebugLiftTime = 2.0f;
#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), Center, Sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLiftTime);

#endif //  ENABLE_DRAW_DEBUG

	// Hit됐으면,
	if (bHit == true)
	{
		for (FOverlapResult result : overlapResults)
		{
			if (Cast<ACHumanoidMonster>(result.GetActor()))
			{
				// UE_LOG(LogTemp, Warning, (*result.GetActor()->GetName()));
				// CLog::Print((*result.GetActor()->GetName()));

				FindAttackTarget = Cast<APawn>(result.GetActor());
				return; 
			}
		}
	}
}

