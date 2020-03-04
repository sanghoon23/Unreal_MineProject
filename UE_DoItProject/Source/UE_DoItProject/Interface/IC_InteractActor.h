#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_InteractActor.generated.h"

UENUM()
enum class InteractType : uint8
{
	JUMPOVER = 0,
	CLIMB = 1,
	END = 2,
};

UINTERFACE(MinimalAPI)
class UIC_InteractActor : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_InteractActor
{
	GENERATED_BODY()

	/* Vritual Function */
public:
	// @param Actor - 상호작용 '당할' Actor
	virtual void BeginInteract(AActor* InteractedActor) = 0;

	// @param Actor - 상호작용 '당할' Actor
	virtual void TickInteract(AActor* InteractedActor) = 0;

	// @param Actor - 상호작용 '당할' Actor
	virtual void EndInteract(AActor* InteractedActor) = 0;

public:
	InteractType GetInteractType() { return Type; }

protected:
	InteractType Type = InteractType::END;
};
