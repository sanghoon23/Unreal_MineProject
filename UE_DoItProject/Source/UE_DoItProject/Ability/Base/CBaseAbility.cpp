#include "CBaseAbility.h"

UCBaseAbility::UCBaseAbility()
{
	AbilityType = EAbilityType::NONE;
}

void UCBaseAbility::StartUseTimerAbility()
{
	check(AppliedActor);
}

void UCBaseAbility::TickUseTimerAbility(float DeltaTime)
{
	//@Apply Time
	AbilityValue.Timer -= DeltaTime;

	if (AbilityValue.Timer < OpacityLinearTimer)
	{
		UpdateUIColorAndOpacity();
	}
}

void UCBaseAbility::EndUseTimerAbility()
{
	check(AppliedActor);
}

void UCBaseAbility::OverlapAbility(class UCBaseAbility* Ability)
{
	check(Ability);
	InitUIColorAndOpacity(); //@Init Opacity
}

void UCBaseAbility::TimerRunning(float DeltaTime)
{
	AbilityValue.Timer -= DeltaTime;
}

void UCBaseAbility::InitUIColorAndOpacity()
{
	ColorAndOpacity = FLinearColor(FVector4(1.0f));
	bLinerColorDir = true;
}

void UCBaseAbility::UpdateUIColorAndOpacity()
{
	float Opacity = ColorAndOpacity.A;

	(bLinerColorDir == true)
		? Opacity += OpacityLinearSpeed
		: Opacity -= OpacityLinearSpeed;

	if (Opacity > 1.0f || Opacity < 0.1f)
	{
		bLinerColorDir = !(bLinerColorDir);
	}

	ColorAndOpacity.A = Opacity;
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
