#include "CPL_TargetingSystem.h"
#include "Global.h"

#include "Charactor/Monster/Base/CHumanoidMonster.h"
#include "Charactor/Player/CPlayer.h"

//UI
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_TargetInfo.h"

UCPL_TargetingSystem::UCPL_TargetingSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_TargetingSystem::BeginPlay()
{
	Super::BeginPlay();

	//@Set Player
	Player = Cast<ACPlayer>(GetOwner());

	//@UI
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr)
	{
		AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		check(MainHUD);
		TargetInfoWidget = MainHUD->GetWidgetTargetInfo();
	}
}


void UCPL_TargetingSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController != nullptr)
	{
		if(PlayerController->IsInputKeyDown(EKeys::Escape))
		{
			//@Target NULL 시키기.
			FindAttackTarget = nullptr;

			//@Widget UnVisible
			TargetInfoWidget->WigetUnVisible();
		}
	}
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

				//@Target
				FindAttackTarget = Cast<APawn>(result.GetActor());

				//@Widget Visible
				TargetInfoWidget->WigetVisible();
				return; 
			}
		}
	}
}

