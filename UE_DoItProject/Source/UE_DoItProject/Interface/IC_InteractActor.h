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

/* 상호작용(Interactive) 에 쓰일 Data 모음 */
// @Waring -
// 특정 InteractType 마다 해당 Type 에서 쓰는 값들은 Setting 해주어야 함.
USTRUCT()
struct FInteractiveData
{
	GENERATED_USTRUCT_BODY()

	float JumpOverSpeed = 0.0f;

	//...
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
	void SetInteractiveData(const FInteractiveData& FData) { InteractiveData = FData; }

protected:
	InteractType Type = InteractType::END;

	FInteractiveData InteractiveData;
};
