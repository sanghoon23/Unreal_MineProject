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
		/*
		적용될 ItemEvent 를 랜덤으로 스폰 (범위로 설정)
		//@param Start	- EItemEventType 의 첫번째 범위
		//@param End	- EItemEventType 의 마지막 범위
		*/
		static ACItem_Event* RandomSpawnAppliedItemEvent
		(
			AActor* DoingActor, EItemEventType Start, EItemEventType End,
			FVector Location = FVector(0.0f), FRotator Rotation = FRotator(0.0f)
		);
};
