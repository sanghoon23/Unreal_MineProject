#include "C_BaseAttackState.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

UC_BaseAttackState::UC_BaseAttackState()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UC_BaseAttackState::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	CLog::Print(GetOwner()->GetName());
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;

		bAttackPossible = false; //@AttackPossible
	});

	//IC_Charactor->OnActionResetState.AddUObject(this, &UC_BaseAttackState::Del_ResetState);
	//BeginAttackDeleFunc.AddUObject(this, &UC_BaseAttackState::Del_BeginAttack);

	// Set Delegate "Begin Attack" - IIC_BaseAttack
	BeginAttackDeleFunc.AddLambda([&]()
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	//EndAttackDeleFunc.AddUObject(this, &UC_BaseAttackState::Del_EndAttack);

	#pragma endregion

}

void UC_BaseAttackState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UC_BaseAttackState::EndAttack()
{
	bAttackMode = false;
	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;

	bAttackPossible = false;
}

//void UC_BaseAttackState::Del_ResetState(AActor* Acto)
//{
//	bAttackMode = false;
//	bAttacking = false;
//	bComboCheck = false;
//	CurrentComboNum = 0;
//
//	bAttackPossible = false; //@AttackPossible
//}
//
//void UC_BaseAttackState::Del_BeginAttack()
//{
//	bAttacking = true;
//	bAttackMode = true;
//}
//
//void UC_BaseAttackState::Del_EndAttack()
//{
//	bAttackMode = false;
//	bAttacking = false;
//	bComboCheck = false;
//	CurrentComboNum = 0;
//
//	bAttackPossible = false;
//}


