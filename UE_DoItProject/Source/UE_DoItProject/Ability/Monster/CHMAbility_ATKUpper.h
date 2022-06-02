#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CHMAbility_ATKUpper.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHMAbility_ATKUpper 
	: public UCBaseAbility
{
	GENERATED_BODY()

#pragma region Reflection
private:

#pragma endregion

public:
	UCHMAbility_ATKUpper();

	/* Virtual Function */
public:
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility() override;

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	virtual void ApplyAbility() override;
	
	/* Member */
public:
	void SetATKUpperCoefficient(float fValue) { ATKUpperCoefficient = fValue; }

private:
	float ATKUpperCoefficient = 0.0f;
};
