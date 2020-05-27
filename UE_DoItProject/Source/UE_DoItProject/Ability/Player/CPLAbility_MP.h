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
	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	virtual void ApplyAbility();
};
