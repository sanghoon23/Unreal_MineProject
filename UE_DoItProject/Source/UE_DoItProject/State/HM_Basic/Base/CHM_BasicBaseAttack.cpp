#include "CHM_BasicBaseAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

UCHM_BasicBaseAttack::UCHM_BasicBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCHM_BasicBaseAttack::BeginPlay()
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
	BeginAttackDeleFunc.AddLambda([&]()
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddLambda([&]()
	{
		bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;
	});
	
}


void UCHM_BasicBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCHM_BasicBaseAttack::BeginAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCHM_BasicBaseAttack::EndAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);

	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;
}

void UCHM_BasicBaseAttack::OnComboSet(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCHM_BasicBaseAttack::CheckAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCHM_BasicBaseAttack::AttackImpulse(AActor * DoingActor, float intensity)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCHM_BasicBaseAttack::CheckProcedural(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}


