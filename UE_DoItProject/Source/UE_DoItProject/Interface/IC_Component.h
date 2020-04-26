#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_Component.generated.h"

UINTERFACE(MinimalAPI)
class UIC_Component : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_Component
{
	GENERATED_BODY()
	
public:
	/* Tick 실행 여부 - PrimaryComponentTick.bCanEverTick = bRunning */
	virtual void IsRunTick(bool bRunning) = 0;

};
