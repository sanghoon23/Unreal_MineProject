#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Air.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CHM_AssaFourAttack.generated.h"

UENUM()
enum class EHM_AssaFourComboType : uint8
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
class UE_DOITPROJECT_API UCHM_AssaFourAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()
	
private:
	const float NextSectionPlayRate = 0.9f;

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

#pragma endregion

public:
	UCHM_AssaFourAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual - IC_BaseAttack */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual bool IsLastCombo() const override;

	//virtual bool IsLastCombo() const override;
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

	float AttackRadius = 350.0f;

	bool bSettingPlayRate = false;
	float StartSectionLength = 0.0f;
	float EndSectionLength = 0.0f;

#pragma endregion

};
