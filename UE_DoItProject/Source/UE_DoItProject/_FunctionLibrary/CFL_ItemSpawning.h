#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/Base/CDisplayItem.h"
#include "Item/Base/CItem_Event.h"
#include "Item/Base/CItem_Hand.h"

#include "CFL_ItemSpawning.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCFL_ItemSpawning 
	: public UObject
{
	GENERATED_BODY()

public:
	UCFL_ItemSpawning();

public:
	UFUNCTION(Category = "Spawning")
		static ACItem_Event* SpawnItemEventOrNull(AActor* DoingActor, EItemEventType Type, FVector Location, FRotator Rotation);
};
