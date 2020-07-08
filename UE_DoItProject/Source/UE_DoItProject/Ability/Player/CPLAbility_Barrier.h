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
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility() override;

	/* Timer �� ����� �� AbilityComponent ���� Tick */
	virtual void TickUseTimerAbility(float DeltaTime) override;

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility() override;

	virtual void OverlapAbility(class UCBaseAbility* Ability) override;

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	//virtual void ApplyAbility();

};
