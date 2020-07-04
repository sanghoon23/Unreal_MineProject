#include "C_BaseAbilityComp.h"
#include "Global.h"

#include "Ability/Base/CBaseAbility.h"

UC_BaseAbilityComp::UC_BaseAbilityComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_BaseAbilityComp::BeginPlay()
{
	Super::BeginPlay();
}

void UC_BaseAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TArray<EAbilityType> RemoveTypes; //제거할 컨테이너
	for (auto& MapValue : AddAbilityMap)
	{
		EAbilityType Type = MapValue.Value->GetAbilityType();

		//@Tick
		MapValue.Value->TickUseTimerAbility(DeltaTime);
		MapValue.Value->TimerRunning(DeltaTime);

		//@Time Check
		bool bTimeOut = (MapValue.Value)->IsTimeOut();
		if (bTimeOut == true)
		{
			//@End
			MapValue.Value->EndUseTimerAbility();
			RemoveTypes.Add(Type);
			//CLog::Print(L"TimeOut Add REmove");
		}
	}

	//@제거
	for (EAbilityType& Type : RemoveTypes)
	{
		//CLog::Print(L"REmove");
		AddAbilityMap.Remove(Type);
	}
}

void UC_BaseAbilityComp::AddAbility(UCBaseAbility* Ability)
{
	check(Ability);

	Ability->SetAppliedActor(GetOwner());

	bool bUsingTimer = Ability->GetUsingTimer();
	if (bUsingTimer == true)
	{
		EAbilityType Type = Ability->GetAbilityType();
		UCBaseAbility** Origin = AddAbilityMap.Find(Type);
		if (Origin != nullptr)
		{
			//@Overlap
			(*Origin)->OverlapAbility(Ability);
			//AddAbilityMap.Remove(Type);
		}
		else
		{
			Ability->StartUseTimerAbility();
			AddAbilityMap.Add(Type, Ability);
		}
	}
	else
	{
		Ability->ApplyAbility();
	}
}

void UC_BaseAbilityComp::GetAbilities(TArray<UCBaseAbility*>& OutArray)
{
	for (auto& Ability : AddAbilityMap)
	{
		OutArray.Add(Ability.Value);
	}
}
