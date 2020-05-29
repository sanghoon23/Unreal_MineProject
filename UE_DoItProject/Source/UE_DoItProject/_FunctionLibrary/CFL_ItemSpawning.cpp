#include "CFL_ItemSpawning.h"
#include "CLog.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

#include "Particles/ParticleSystemComponent.h"

#include "Item/Event/CItem_RecoveryHP.h"
#include "Item/Event/CItem_RecoveryMP.h"
#include "Item/Event/CItem_Faster.h"
#include "Item/Event/CItem_Barrier.h"

UCFL_ItemSpawning::UCFL_ItemSpawning()
{
}

ACItem_Event * UCFL_ItemSpawning::RandomSpawnAppliedItemEvent(AActor * DoingActor, EItemEventType Start, EItemEventType End, FVector Location, FRotator Rotation)
{
	UWorld* const World = DoingActor->GetWorld();

	FTransform Transform = FTransform::Identity;
	Transform.SetLocation(Location);
	Transform.SetRotation(FQuat(Rotation));

	FActorSpawnParameters Params;
	Params.Owner = DoingActor;

	ACItem_Event* Ret = nullptr;

	EItemEventType RandomType;
	int RandomValue = 0;
	RandomValue =
		UKismetMathLibrary::RandomIntegerInRange(static_cast<int>(Start), static_cast<int>(End));
	FMath::Clamp(RandomValue, static_cast<int>(Start), static_cast<int>(End));
	RandomType = static_cast<EItemEventType>(RandomValue);

	//@RECOVERY HP
	if (RandomType == EItemEventType::RECOVERY_HP)
	{
		Ret = World->SpawnActor<ACItem_RecoveryHP>(ACItem_RecoveryHP::StaticClass(), Transform, Params);
	}
	//@RECOVERY MP
	else if (RandomType == EItemEventType::RECOVERY_MP)
	{
		Ret = World->SpawnActor<ACItem_RecoveryMP>(ACItem_RecoveryMP::StaticClass(), Transform, Params);
	}
	//@FASTER
	else if (RandomType == EItemEventType::FASTER)
	{
		Ret = World->SpawnActor<ACItem_Faster>(ACItem_Faster::StaticClass(), Transform, Params);
	}
	//@BARRIER
	else if (RandomType == EItemEventType::BARRIER)
	{
		Ret = World->SpawnActor<ACItem_Barrier>(ACItem_Barrier::StaticClass(), Transform, Params);
	}
	else
		UE_LOG(LogTemp, Warning, L"UCFL_ItemSPawning Don't Find Type!!");

	return Ret;
}
