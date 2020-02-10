#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_BaseAction.h"

#include "IC_ActionComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_ActionComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_ActionComp
{
	GENERATED_BODY()

public:
	/* Action Component �� ���� IBaseAction Interface �� ������ */
	virtual IIC_BaseAction* GetIBaseAction(uint8 Type) = 0;
};
