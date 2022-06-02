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
	/* Timer 를 사용한 능력치 적용 시작 */
	virtual void StartUseTimerAbility() override;

	/* Timer 를 사용한 능력치 적용 종료 */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer 를 사용하지 않고 영구적으로 능력치 적용 */
	virtual void ApplyAbility() override;
	
	/* Member */
public:
	void SetATKUpperCoefficient(float fValue) { ATKUpperCoefficient = fValue; }

private:
	float ATKUpperCoefficient = 0.0f;
};
