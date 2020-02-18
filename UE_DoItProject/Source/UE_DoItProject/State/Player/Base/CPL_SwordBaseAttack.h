#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_SwordBaseAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SwordBaseAttack
	: public UActorComponent, public IIC_BaseAttack
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> SwordAttackMontages;

	#pragma endregion

public:	
	UCPL_SwordBaseAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack() override;
	virtual void OnComboSet(AActor * DoingActor) override;

	virtual void AttackOtherPawn() override;
	virtual void ImpulseAttack(float intensity) override;
	virtual void CheckProcedural() override;

public:

	#pragma	region Member
public:
	bool GetAttacking() const override { return bAttacking; }
	void SetAttacking(bool bValue) override { bAttacking = bValue; };

	bool GetAttackMode()const  override { return bAttackMode; }
	void SetAttackMode(bool bValue) override { bAttackMode = bValue; };

	bool GetComboCheck() const override { return bComboCheck; }
	bool IsLastCombo() const override { return false; }

	virtual void SetAttackPossible() { bAttackPossible = true; }
	virtual bool IsAttackPossible() const { return bAttackPossible; }

protected:
	class ACPlayer*		Player;

	UINT				CurrentComboNum = 0;
	UINT				MaxComboNum = 0;

	/* @���ݹ���, Target�� �� ���� �ȿ� ���;����� ���ݰ��� */
	float AttackRange = 200.0f;
	bool bAttackCall = false;
	bool bAttackPossible = false; // ���ݰ�������.

	bool bAttackMode	= false;

	bool bAttacking		= false;
	bool bComboCheck	= false;

#pragma endregion
};
