#include "CPlayerAttackComp.h"
#include "Global.h"

UCPlayerAttackComp::UCPlayerAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCPlayerAttackComp::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCPlayerAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

IIC_BaseAttack * UCPlayerAttackComp::GetCurrentIBaseAttack()
{
	IIC_BaseState* BaseState = Cast<IIC_BaseState>(CurrentAttackState);
	IfNullRetResult(BaseState, nullptr);

	return BaseState->GetIBaseAttack();
}
