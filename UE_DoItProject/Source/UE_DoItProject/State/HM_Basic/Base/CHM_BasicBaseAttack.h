#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_BasicBaseAttack.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCHM_BasicBaseAttack 
	: public UActorComponent, public IIC_BaseAttack
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	#pragma endregion

public:	
	UCHM_BasicBaseAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/* Virtual Function Override */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack() override;
	virtual void OnComboSet(AActor * DoingActor) override;

	virtual void AttackOtherPawn() override;
	virtual void ImpulseAttack(float intensity) override;
	virtual void CheckProcedural() override;

	#pragma	region Member
public:
	virtual bool GetAttacking() const override { return bAttacking; }
	virtual void SetAttacking(bool bValue) override { bAttacking = bValue; };

	virtual bool GetAttackMode() const override { return bAttackMode; }
	virtual void SetAttackMode(bool bValue) override { bAttackMode = bValue; };

	virtual bool GetComboCheck() const override { return bComboCheck; }
	virtual bool IsLastCombo() const override { return false; }

protected:
	// @공격범위, Target과 이 범위 안에 들어와야지만 공격가능 - Default
	float AttackRange	= 200.0f;

	UINT				CurrentComboNum = 0;
	UINT				MaxComboNum = 0;

	bool bAttackMode = false;

	bool bAttacking = false;
	bool bComboCheck = false;

#pragma endregion
};
