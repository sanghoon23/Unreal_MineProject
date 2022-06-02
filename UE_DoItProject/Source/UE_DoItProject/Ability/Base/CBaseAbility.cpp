#include "CBaseAbility.h"
#include "Global.h"

UCBaseAbility::UCBaseAbility()
{
	AbilityType = EAbilityType::NONE;
}

void UCBaseAbility::Copy(const UCBaseAbility * const In)
{
	//OnDelStartTimerAbility.RemoveAll(this);
	//OnEndTimerAbility.RemoveAll(this); //기존의 것 제거
	//OnDelStartTimerAbility = In->OnDelStartTimerAbility;
	//OnEndTimerAbility = In->OnEndTimerAbility;

	ColorAndOpacity = In->ColorAndOpacity;
	TintColor = In->TintColor;
	bLinerColorDir = In->bLinerColorDir;
	AbilityType = In->GetAbilityType();

	SetAbilityValue(In->GetAbilityValue());
	SetAppliedActor(In->AppliedActor);

	bUsingColorAndOpacity = In->bUsingColorAndOpacity;
	OpacityLinearTimer = In->OpacityLinearTimer;
	OpacityLinearSpeed = In->OpacityLinearSpeed;
}

void UCBaseAbility::StartUseTimerAbility()
{
	check(AppliedActor);

	//@Onwer 가 다를 수 있음
	OnDelStartTimerAbility.Broadcast(AppliedActor);
}

void UCBaseAbility::TickUseTimerAbility(float DeltaTime)
{
	if (AbilityValue.Timer < OpacityLinearTimer && bUsingColorAndOpacity == true)
	{
		UpdateUIColorAndOpacity();
	}

	/* 220506_ */
	TimerRunning(DeltaTime);
}

void UCBaseAbility::EndUseTimerAbility()
{
	check(AppliedActor);

	//@Onwer 가 다를 수 있음
	OnEndTimerAbility.Broadcast(AppliedActor);
}

void UCBaseAbility::OverlapAbility(class UCBaseAbility* Ability)
{
	check(Ability);
	InitUIColorAndOpacity(); //@Init Opacity

	Copy(Ability);
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
	if (AbilityValue.Timer <= 0.0f) return true;
	
	return false;
}

void UCBaseAbility::SetAbilityValue(EAbilitySort Sort, float Val, bool bUsingTimer, float TimerVal)
{
	AbilityValue.Sort = Sort;
	AbilityValue.bTimer = bUsingTimer;
	AbilityValue.Timer = TimerVal;
	AbilityValue.Value = Val;
}

void UCBaseAbility::SetAbilityValue(const FAbilityValue & Input)
{
	AbilityValue.Sort = Input.Sort;
	AbilityValue.bTimer = Input.bTimer;
	AbilityValue.Timer = Input.Timer;
	AbilityValue.Value = Input.Value;
}

void UCBaseAbility::SetAppliedActor(AActor * Actor)
{
	check(Actor);
	AppliedActor = Actor;
}
