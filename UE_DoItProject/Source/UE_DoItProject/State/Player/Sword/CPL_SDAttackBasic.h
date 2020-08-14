#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "Interface/IC_Component.h"

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
	: public UCPL_SwordBaseAttack, public IIC_Component
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	UCPL_SDAttackBasic();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Function - IC_BaseAttack */
public:
	void BeginAttack(AActor * DoingActor) override;
	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;


	void AttackOtherPawn(UCDamageType_Base* DamageType) override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

	#pragma region Member
private:
	float AttackRadius = 100.0f;

	#pragma endregion
};
