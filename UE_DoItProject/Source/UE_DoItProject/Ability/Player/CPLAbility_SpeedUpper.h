#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_SpeedUpper.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_SpeedUpper
	: public UCBaseAbility
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	UCPLAbility_SpeedUpper();

	/* Virtual Function */
public:
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility() override;

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	//virtual void ApplyAbility();

};
