#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_BaseAttack.h"

#include "IC_AttackComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_AttackComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_AttackComp
{
	GENERATED_BODY()

public:
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() { return nullptr; }
	// virtual IIC_BaseAttack* GetAttackStateMember(uint8 Num) { return nullptr; }
};
