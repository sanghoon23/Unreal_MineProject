#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"

#include "DamageType/CDamageType_Air.h"
#include "DamageType/CDamageType_AirAttack.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CPL_SDAttackUpper.generated.h"

UENUM()
enum class USD_UpperAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	COMBO_FOUR = 3,
	COMBO_FIVE = 4,
	COMBO_SIX = 5,
	END = 6,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackUpper 
	: public UCPL_SwordBaseAttack
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	//UPROPERTY(VisibleAnywhere, Category = "DamageType")
	//	TSubclassOf<UCDamageType_Normal> DT_Normal;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Air*			DT_Air;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_AirAttack*		DT_AirAttack;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_StrongAttack;

	#pragma endregion

public:
	UCPL_SDAttackUpper();
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
	void ImpulseAttack(float intensity) override {}
	void CheckProcedural() override {}

private:
	void LookAtTarget(AActor* Target);
	void ActorLocateFrontTarget(AActor* Target);

	#pragma region Member
private:
	float AttackRadius = 100.0f;

	#pragma endregion
};
