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
	/* 능력치 추가 */
	virtual void AddAbility(UCBaseAbility* Ability) = 0;

	/* 능력치 가져오기 */
	virtual void GetAbilities(TArray<UCBaseAbility*>& OutArray) = 0;
};
