#include "CFL_ItemSpawning.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystemComponent.h"

#include "Item/Event/CItem_RecoveryHP.h"
//TODO : Ãß°¡

UCFL_ItemSpawning::UCFL_ItemSpawning()
{
}

ACItem_Event* UCFL_ItemSpawning::SpawnItemEventOrNull(AActor* DoingActor, EItemEventType Type, FVector Location, FRotator Rotation)
{
	UWorld* const World = DoingActor->GetWorld();

	FTransform Transform = FTransform::Identity;
	Transform.SetLocation(Location);
	Transform.SetRotation(FQuat(Rotation));

	FActorSpawnParameters Params;
	Params.Owner = DoingActor;

	ACItem_Event* Ret = nullptr;
	//@RECOVERY HP
	if (Type == EItemEventType::RECOVERY_HP)
	{
		Ret = World->SpawnActor<ACItem_RecoveryHP>(ACItem_RecoveryHP::StaticClass(), Transform, Params);
	}
	else
		UE_LOG(LogTemp, Warning, L"UCFL_ItemSPawning Don't Find Type!!");


	return Ret;
}
