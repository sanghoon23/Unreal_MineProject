#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_MP.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_MP 
	: public UCBaseAbility
{
	GENERATED_BODY()
	
public:
	UCPLAbility_MP();

	/* Virtual Function */
public:
	/* Timer 를 사용하지 않고 영구적으로 능력치 적용 */
	virtual void ApplyAbility();
};
