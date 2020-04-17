#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CPL_SDAttackFinish.generated.h"

UENUM()
enum class USD_FinalAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackFinish 
	: public UCPL_SwordBaseAttack
{
	GENERATED_BODY()

#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DT_Noraml;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_StrongAttack;

#pragma endregion

public:
	UCPL_SDAttackFinish();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function */
public:
	void BeginAttack(AActor * DoingActor) override;
	void EndAttack() override;

	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;

	/* Virtual */
public:
	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override {}
	void CheckProcedural() override {}
	
	#pragma region Member
private:
	float AttackRadius = 100.0f;
	float StartAttackActionDistance = 200.0f;

	APlayerController* PlayerController;
	#pragma endregion
};
