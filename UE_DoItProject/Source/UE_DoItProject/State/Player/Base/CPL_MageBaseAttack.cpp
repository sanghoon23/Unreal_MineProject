#include "CPL_MageBaseAttack.h"
#include "Global.h"

#include "Interface/IC_BaseAttack.h"

UCPL_MageBaseAttack::UCPL_MageBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPL_MageBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCPL_MageBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MageBaseAttack::BeginAttack(AActor * Actor)
{
}

void UCPL_MageBaseAttack::EndAttack(AActor * Actor)
{
}

void UCPL_MageBaseAttack::OnComboSet(AActor * Actor)
{
}

void UCPL_MageBaseAttack::CheckAttack(AActor * Actor)
{
}

void UCPL_MageBaseAttack::AttackImpulse(AActor * DoingActor, float intensity)
{
}

void UCPL_MageBaseAttack::CheckProcedural(AActor * DoingActor)
{
}

