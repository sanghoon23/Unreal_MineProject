#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "Interface/IC_Component.h"

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
	: public UCPL_MageBaseAttack, public IIC_Component
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*		DT_Normal;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_StrongAttack;

	#pragma endregion

public:
	UCPL_MGAttackBasic();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Pure Virtual Function - IC_BaseAttack */
public:
	void BeginAttack(AActor * DoingActor) override;
	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;

	/* Virtual */
public:
	void AttackOtherPawn(UCDamageType_Base* DamageType) override;

	#pragma region Member
private:
	float AttackRadius = 100.0f;

	#pragma endregion
};
