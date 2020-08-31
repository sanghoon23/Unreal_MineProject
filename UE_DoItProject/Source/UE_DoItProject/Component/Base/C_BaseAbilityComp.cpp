#include "C_BaseAbilityComp.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
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

	////@죽었다면 Return
	//IIC_Charactor* OwnerCharactor = Cast<IIC_Charactor>(GetOwner());
	//if (OwnerCharactor != nullptr)
	//{
	//	IfTrueRet(OwnerCharactor->IsDeath() == true);
	//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TArray<EAbilityType> RemoveTypes; //제거할 컨테이너
	for (auto& MapValue : AddAbilityMap)
	{
		EAbilityType Type = MapValue.Value->GetAbilityType();

		//@Tick
		MapValue.Value->TickUseTimerAbility(DeltaTime);

		bool bTimeOut = false;
		if (MapValue.Value->GetUsingTimer() == true)
		{
			MapValue.Value->TimerRunning(DeltaTime);
			bTimeOut = (MapValue.Value)->IsTimeOut();
		}

		//@Time Check
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
	EAbilitySort Sort = Ability->GetAbilitySort();
	if (Sort == EAbilitySort::SAVEARRAY)
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
	if (Sort == EAbilitySort::IMMEDITATEAPPLY)
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
