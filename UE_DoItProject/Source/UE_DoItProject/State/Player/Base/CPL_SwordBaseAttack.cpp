#include "CPL_SwordBaseAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

UCPL_SwordBaseAttack::UCPL_SwordBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCPL_SwordBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// Set Delegate "On Hit" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		// bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;
	});

	// Set Delegate "Begin Attack" - IIC_BaseAttack
	BeginAttackDeleFunc.AddLambda([&]()
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddLambda([&]()
	{
		// bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;
	});
}


void UCPL_SwordBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SwordBaseAttack::BeginAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_SwordBaseAttack::EndAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);

	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;
}

void UCPL_SwordBaseAttack::OnComboSet(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_SwordBaseAttack::CheckAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_SwordBaseAttack::AttackImpulse(AActor * DoingActor, float intensity)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_SwordBaseAttack::CheckProcedural(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

