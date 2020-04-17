#include "C_BaseAttackState.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

UC_BaseAttackState::UC_BaseAttackState()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UC_BaseAttackState::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;

		bAttackPossible = false; //@AttackPossible
	});

	// Set Delegate "Begin Attack" - IIC_BaseAttack
	BeginAttackDeleFunc.AddLambda([&]()
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddUObject(this, &UC_BaseAttackState::EndAttack);

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

