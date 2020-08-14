#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Air.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CHM_AssaSecondAttack.generated.h"

UENUM()
enum class EHM_AssaSecondComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	COMBO_TWO = 2,
	COMBO_THREE = 3,
	COMBO_FOUR = 4,
	COMBO_FIVE = 5,
	COMBO_SIX = 6,
	END = 7,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_AssaSecondAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Air*	DT_Air;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack* DT_Strong;

#pragma endregion

public:
	UCHM_AssaSecondAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual - IC_BaseAttack */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual bool IsLastCombo() const override;

	virtual void AttackOtherPawn(UCDamageType_Base* DamageType) override;

	/* Function */
private:
	void BeginBeatedFunction(AActor* Subject);
	void EndBeatedFunction(AActor* Subject);

#pragma region Member
private:
	class ACHM_Assassin* HM_Assassin;
	class IIC_Charactor* I_Charactor;

	// @공격범위, Target과 이 범위 안에 들어와야지만 공격가능 - Default
	float AttackRange = 200.0f;

	float AttackRadius = 200.0f;

#pragma endregion

};
