#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "DamageType/CDamageType_Normal.h"

#include "CPL_SDAttackBasic.generated.h"

UENUM()
enum class USD_BasicAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackBasic
	: public UCPL_SwordBaseAttack
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DT_Noraml;

	#pragma endregion

public:
	UCPL_SDAttackBasic();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Function */
public:
	void BeginAttack(AActor * DoingActor) override;
	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;


	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

	#pragma region Member
private:
	float AttackRadius = 100.0f;

	#pragma endregion
};
