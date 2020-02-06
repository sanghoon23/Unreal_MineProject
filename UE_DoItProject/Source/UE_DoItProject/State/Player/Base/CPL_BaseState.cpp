#include "CPL_BaseState.h"
#include "Global.h"

UCPL_BaseState::UCPL_BaseState()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_BaseState::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_BaseState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

IIC_BaseAttack * UCPL_BaseState::GetIBaseAttack()
{
	// if (StateType != StateType::ATTACK) return nullptr;
	return nullptr;
}

