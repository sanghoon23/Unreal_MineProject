#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAttack.h"

#include "C_BaseAttackState.generated.h"

UCLASS()
class UE_DOITPROJECT_API UC_BaseAttackState
	: public UActorComponent, public IIC_BaseAttack
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:
	UC_BaseAttackState();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Virtual Function Override */
public:
	virtual void BeginAttack(AActor * DoingActor) override {}
	virtual void EndAttack() override;
	virtual void OnComboSet(AActor * DoingActor) override {}

	virtual void AttackOtherPawn() override {}
	virtual void ImpulseAttack(float intensity) override {}
	virtual void CheckProcedural() override {}

public:
	virtual bool IsLastCombo() const override { return false; }

	bool GetAttacking() const override { return bAttacking; }
	void SetAttacking(bool bValue) override { bAttacking = bValue; };

	bool GetAttackMode()const  override { return bAttackMode; }
	void SetAttackMode(bool bValue) override { bAttackMode = bValue; };

	bool GetComboCheck() const override { return bComboCheck; }

	/* Function */
protected:
	///* AutoAttack �� ���������, ������ ��Ȳ���� ����. Ex) FinishedAttack */
	//void StopAutoAttack() { bAutoAttackCall = false; }

	//* ���� �������� �Ļ� Ŭ�������� �˱� ���� ��� */
	virtual bool GetAttackPossible() const { return bAttackPossible; }

	#pragma	region Member
protected:
	UINT					CurrentComboNum = 0;
	UINT					MaxComboNum		= 0;

	bool bAttackMode		= false;
	bool bAttacking			= false;
	bool bComboCheck		= false;

	bool bAttackPossible	= false;	// ���ݰ�������.

private:

	#pragma endregion
		
};
