#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_StrongAttack.h"

#include "CHM_AssaSixAttack.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCHM_AssaSixAttack
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

#pragma endregion

public:
	UCHM_AssaSixAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual - IC_BaseAttack */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	//virtual bool IsLastCombo() const override;

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

	// @���ݹ���, Target�� �� ���� �ȿ� ���;����� ���ݰ��� - Default
	float AttackRange = 200.0f;

	float AttackRadius = 200.0f;

#pragma endregion
	
};
