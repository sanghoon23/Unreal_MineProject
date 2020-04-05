#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"

#include "CPL_SwordBaseAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SwordBaseAttack
	: public UC_BaseAttackState
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(VisibleAnywhere, Category = "System")
		class UCS_AttackDecision* AttackDecision;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> SwordAttackMontages;

	#pragma endregion

public:	
	UCPL_SwordBaseAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Virutal Function */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack() override;
	virtual void OnComboSet(AActor * DoingActor) override;

	virtual void AttackOtherPawn() override;
	virtual void ImpulseAttack(float intensity) override;
	virtual void CheckProcedural() override;

protected:


	#pragma	region Member
public:
	
protected:
	class ACPlayer*			Player;

	// @공격범위, Target과 이 범위 안에 들어와야지만 공격가능 - Default
	float AttackRange = 200.0f;

private:
	//@Tick Entry Value
	bool bInputAttackCall = false;

#pragma endregion
};
