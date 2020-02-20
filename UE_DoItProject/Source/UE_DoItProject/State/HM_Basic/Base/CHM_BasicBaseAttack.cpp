#include "CHM_BasicBaseAttack.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

UCHM_BasicBaseAttack::UCHM_BasicBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCHM_BasicBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// Set Delegate "OnAction ResetState" - IIC_Charactor
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

void UCHM_BasicBaseAttack::EndAttack()
{
	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;
}

void UCHM_BasicBaseAttack::OnComboSet(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCHM_BasicBaseAttack::AttackOtherPawn()
{
}

void UCHM_BasicBaseAttack::ImpulseAttack(float intensity)
{
}

void UCHM_BasicBaseAttack::CheckProcedural()
{
}


