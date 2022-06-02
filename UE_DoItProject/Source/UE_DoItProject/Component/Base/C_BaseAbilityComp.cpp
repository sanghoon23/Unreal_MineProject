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

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(I_Charactor);

	//@DeathDelegate Delete Value Container
	I_Charactor->OnDeathDelegate.AddLambda([&]()
	{
		for (auto& MapValue : AddAbilityMap)
		{
			//@End
			MapValue.Value->EndUseTimerAbility();
		}
		AddAbilityMap.Empty();
	});
}

void UC_BaseAbilityComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//#1112_
	//죽었다면 Return - 지우는 부분 (Charactor Delegate Lamda)
	IIC_Charactor* OwnerCharactor = Cast<IIC_Charactor>(GetOwner());
	if (OwnerCharactor != nullptr)
	{
		IfTrueRet(OwnerCharactor->IsDeath() == true);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TArray<EAbilityType> RemoveTypes; //제거할 컨테이너
	for (auto& MapValue : AddAbilityMap)
	{
		EAbilityType Type = MapValue.Value->GetAbilityType();

		//@Tick
		MapValue.Value->TickUseTimerAbility(DeltaTime);

		//@Time Check
		if ((MapValue.Value)->IsTimeOut()== true)
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
			//CLog::Print(L"OverlapAbility");
			//AddAbilityMap.Remove(Type);
		}
		else
		{
			//CLog::Print(L"StartUseTimerAbility");
			Ability->StartUseTimerAbility();
			AddAbilityMap.Add(Type, Ability);
		}
	}
	else if (Sort == EAbilitySort::IMMEDITATEAPPLY)
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
