#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_HP.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_HP 
	: public UCBaseAbility
{
	GENERATED_BODY()
	
public:
	UCPLAbility_HP();

	/* Virtual Function */
public:
	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	virtual void ApplyAbility();
};
