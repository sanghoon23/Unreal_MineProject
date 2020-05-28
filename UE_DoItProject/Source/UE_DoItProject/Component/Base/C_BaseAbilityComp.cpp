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
		MapValue.Value->TickUseTimerAbility();
		MapValue.Value->TimerRunning(DeltaTime);

		//@Time Check
		bool bTimeOut = MapValue.Value->IsTimeOut();
		if (bTimeOut == true)
		{
			//@End
			MapValue.Value->EndUseTimerAbility();
			RemoveTypes.Add(Type);
		}
	}

	//@제거
	for (EAbilityType& Type : RemoveTypes)
	{
		AddAbilityMap.Remove(Type);
	}
}

void UC_BaseAbilityComp::AddAbility(UCBaseAbility* Ability)
{
	check(Ability);

	bool bUsingTimer = Ability->GetUsingTimer();
	if (bUsingTimer == true)
	{
		EAbilityType Type = Ability->GetAbilityType();
		bool bKeyExist = AddAbilityMap.Contains(Type);
		if (bKeyExist == true)
		{
			//@Overlap
			UCBaseAbility** Origin = AddAbilityMap.Find(Type);
			(*Origin)->OverlapAbility(Ability);
		}
		else //TMap Empalce
		{
			//@Start
			Ability->StartUseTimerAbility();
			AddAbilityMap.Emplace(Type, Ability);
		}
	}
	else
	{
		Ability->ApplyAbility();
	}
}
