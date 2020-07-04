#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Ability/Base/CBaseAbility.h"

#include "IC_AbilityComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_AbilityComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_AbilityComp
{
	GENERATED_BODY()
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function */

public:
	/* �ɷ�ġ �߰� */
	virtual void AddAbility(UCBaseAbility* Ability) = 0;

	/* �ɷ�ġ �������� */
	virtual void GetAbilities(TArray<UCBaseAbility*>& OutArray) = 0;
};
