#include "CPL_SwordAttackComp.h"
#include "Global.h"

UCPL_SwordAttackComp::UCPL_SwordAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_SwordAttackComp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_SwordAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

IIC_BaseAttack * UCPL_SwordAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	IfTureRetResult
	(
		CurrentType > SwordStateArray.Num() - 1,
		nullptr //@Return
	);

	return Cast<IIC_BaseAttack>(SwordStateArray[CurrentType]);
}

