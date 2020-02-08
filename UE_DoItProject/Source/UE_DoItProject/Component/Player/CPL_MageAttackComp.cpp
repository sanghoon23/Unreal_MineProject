#include "CPL_MageAttackComp.h"
#include "Global.h"

UCPL_MageAttackComp::UCPL_MageAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_MageAttackComp::BeginPlay()
{
	Super::BeginPlay();
}


void UCPL_MageAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

IIC_BaseAttack * UCPL_MageAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	IfTureRetResult
	(
		CurrentType > MageStateArray.Num() - 1, 
		nullptr //@Return
	);

	return Cast<IIC_BaseAttack>(MageStateArray[CurrentType]);
}

