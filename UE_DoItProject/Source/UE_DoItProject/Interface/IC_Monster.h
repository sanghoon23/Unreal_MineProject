#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_Charactor.h"

#include "IC_Monster.generated.h"

UINTERFACE(MinimalAPI)
class UIC_Monster : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_Monster
{
	GENERATED_BODY()

	/* Member */
public:
	void SetAIRunningPossible(bool bValue) { bAIRunningPossible = bValue; }
	bool GetAIRunningPossible() const { return bAIRunningPossible; }

private:
	bool bAIRunningPossible = true;
};
