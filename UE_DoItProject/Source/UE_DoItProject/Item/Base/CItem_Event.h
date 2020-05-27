#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CDisplayItem.h"
#include "CItem_Event.generated.h"

UENUM()
enum class EItemEventType : uint8
{
	NONE			= 0,
	RECOVERY_HP		= 1,
	RECOVERY_MP		= 2,
	FASTER			= 3,
	BARRIER			= 4,
	END				= 5,
};

UCLASS()
class UE_DOITPROJECT_API ACItem_Event 
	: public ACDisplayItem
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
		
	#pragma endregion

public:
	ACItem_Event();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/* Function */
public:
	virtual void ApplyEvent(AActor* EventedActor) {};

	/* Member */
public:
	const EItemEventType GetItemEventType() const { return ItemEventType; }

protected:
	EItemEventType ItemEventType = EItemEventType::NONE;
};
