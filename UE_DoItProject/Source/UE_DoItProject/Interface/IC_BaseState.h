#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_BaseAttack.h"

#include "IC_BaseState.generated.h"

UINTERFACE(MinimalAPI)
class UIC_BaseState : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_BaseState
{
	GENERATED_BODY()

public:
	virtual IIC_BaseAttack* GetIBaseAttack() { return nullptr; }
};
