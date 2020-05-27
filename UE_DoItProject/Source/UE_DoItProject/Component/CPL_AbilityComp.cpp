#include "CPL_AbilityComp.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Ability/Base/CBaseAbility.h"

UCPL_AbilityComp::UCPL_AbilityComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCPL_AbilityComp::BeginPlay()
{
	Super::BeginPlay();
}

//TODO : 기존의 속성이 들어온다면?
//TArray 말고 TMap 을 쓰자.

void UCPL_AbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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

void UCPL_AbilityComp::AddAbility(UCBaseAbility* Ability)
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

