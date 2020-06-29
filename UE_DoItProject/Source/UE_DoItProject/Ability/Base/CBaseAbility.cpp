#include "CBaseAbility.h"

UCBaseAbility::UCBaseAbility()
{
	AbilityType = EAbilityType::NONE;
}

const bool UCBaseAbility::IsTimeOut() const
{
	if (AbilityValue.Timer <= 0.0f)
		return true;
	else
		return false;
}

void UCBaseAbility::SetAbilityValue(float Val, bool bUsingTimer, float TimerVal)
{
	AbilityValue.bTimer = bUsingTimer;
	AbilityValue.Timer = TimerVal;
	AbilityValue.Value = Val;
}

void UCBaseAbility::SetAbilityValue(const FAbilityValue & Input)
{
	AbilityValue.bTimer = Input.bTimer;
	AbilityValue.Timer = Input.Timer;
	AbilityValue.Value = Input.Value;
}

void UCBaseAbility::SetAppliedActor(AActor * Actor)
{
	check(Actor);
	AppliedActor = Actor;
}
