#include "CPL_SwordBaseAttack.h"
#include "Global.h"

#include "Interface/IC_BaseAttack.h"

UCPL_SwordBaseAttack::UCPL_SwordBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	StateType = StateType::SWORD;
}


void UCPL_SwordBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_SwordBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SwordBaseAttack::BeginAttack(AActor * Actor)
{
}

void UCPL_SwordBaseAttack::EndAttack(AActor * Actor)
{
}

void UCPL_SwordBaseAttack::OnComboSet(AActor * Actor)
{
}

void UCPL_SwordBaseAttack::CheckAttack(AActor * Actor)
{
}

void UCPL_SwordBaseAttack::AttackImpulse(AActor * DoingActor, float intensity)
{
}

void UCPL_SwordBaseAttack::CheckProcedural(AActor * DoingActor)
{
}

