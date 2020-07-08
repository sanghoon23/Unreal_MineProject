#pragma once

#include "CoreMinimal.h"
#include "Ability/Base/CBaseAbility.h"
#include "CPLAbility_Barrier.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPLAbility_Barrier
	: public UCBaseAbility
{
	GENERATED_BODY()

	#pragma region Reflection
private:

	#pragma endregion

public:
	UCPLAbility_Barrier();

	/* Virtual Function */
public:
	/* Timer 를 사용한 능력치 적용 시작 */
	virtual void StartUseTimerAbility() override;

	/* Timer 를 사용할 때 AbilityComponent 에서 Tick */
	virtual void TickUseTimerAbility(float DeltaTime) override;

	/* Timer 를 사용한 능력치 적용 종료 */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer 를 사용하지 않고 영구적으로 능력치 적용 */
	//virtual void ApplyAbility();

};
