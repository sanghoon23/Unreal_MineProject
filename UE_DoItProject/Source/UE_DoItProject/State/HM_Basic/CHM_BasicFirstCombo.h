#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "DamageType/CDamageType_Normal.h"

#include "CHM_BasicFirstCombo.generated.h"

UENUM()
enum class UHM_BasicFirstComboType : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_BasicFirstCombo 
	: public UC_BaseAttackState
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	NormalDamageType;

	#pragma endregion

public:
	UCHM_BasicFirstCombo();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	//virtual void IsRunTick(bool bRunning) override;

	//
public:
	void BeginAttack(AActor * DoingActor) override;
	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;

	void AttackOtherPawn() override;

	#pragma region Member
private:
	class ACHM_Basic*	HM_Basic;

	// @공격범위, Target과 이 범위 안에 들어와야지만 공격가능 - Default
	float AttackRange = 200.0f;

	float AttackRadius = 100.0f;

	#pragma endregion
};
