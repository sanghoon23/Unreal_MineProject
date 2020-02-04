#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_MontageComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_MontageComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_MontageComp
{
	GENERATED_BODY()

public:
	// TODO : 순수 가상 함수 만들기.
	virtual void PlayAnimation(UINT MonType, UINT PlayAnim, float Speed = 1.5f, bool bAlways = false) {}
};
