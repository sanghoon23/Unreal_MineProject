#include "CPL_MageBaseAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

UCPL_MageBaseAttack::UCPL_MageBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCPL_MageBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// Set Delegate "On Hit" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;
	});

	// Set Delegate "Begin Attack" - IIC_BaseAttack
	BeginAttackDeleFunc.AddLambda([this]()->void
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddLambda([this]()->void
	{
		bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;
	});
}

void UCPL_MageBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MageBaseAttack::BeginAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_MageBaseAttack::EndAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);

	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;
}

void UCPL_MageBaseAttack::OnComboSet(AActor * DoingActor)
{
	check(DoingActor);
	IfNullRet(DoingActor);
}

void UCPL_MageBaseAttack::CheckAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_MageBaseAttack::AttackImpulse(AActor * DoingActor, float intensity)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_MageBaseAttack::CheckProcedural(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

