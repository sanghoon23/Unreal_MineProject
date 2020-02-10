#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_BaseAction.generated.h"

UINTERFACE(MinimalAPI)
class UIC_BaseAction : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_BaseAction
{
	GENERATED_BODY()

public:
	virtual void OnAction(AActor* DoingActor) = 0;
};
