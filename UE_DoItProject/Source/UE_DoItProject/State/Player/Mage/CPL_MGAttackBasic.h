#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CPL_MGAttackBasic.generated.h"

UENUM()
enum class UMG_BasicAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_MGAttackBasic
	: public UCPL_MageBaseAttack
{
	GENERATED_BODY()
	
public:
	UCPL_MGAttackBasic();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function */
public:
	void BeginAttack(AActor * DoingActor) override;
	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;

	/* Virtual */
public:
	void AttackOtherPawn() override;

private:
	void LookAtTarget(AActor* Target);

	#pragma region Member
private:
	float AttackRadius = 100.0f;

	// @DamageType
	UCDamageType_Normal*		DT_Noraml;
	UCDamageType_StrongAttack*	DT_StrongAttack;

	#pragma endregion
};
