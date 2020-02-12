#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_BaseAttack.generated.h"

UINTERFACE(MinimalAPI)
class UIC_BaseAttack : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE(FDele_BeginAttack)
DECLARE_MULTICAST_DELEGATE(FDele_EndAttack)

class UE_DOITPROJECT_API IIC_BaseAttack
{
	GENERATED_BODY()

public:
	FDele_BeginAttack	BeginAttackDeleFunc;
	FDele_EndAttack		EndAttackDeleFunc;

	/* Pure Function */
public:
	// @DoingActor - �����ϴ� Actor(��ü)
	virtual void BeginAttack(AActor* DoingActor) = 0;
	virtual void EndAttack(AActor* DoingActor) = 0;
	virtual void OnComboSet(AActor* DoingActor) = 0;

	virtual bool GetAttacking() const = 0;
	virtual void SetAttacking(bool bValue) = 0;

	virtual bool GetAttackMode() const = 0;
	virtual void SetAttackMode(bool bValue) = 0;

	virtual bool GetComboCheck() const = 0;

	// @Combo �� ������ ������ ��Ȯ�� �˱� ���ؼ�.
	// AI �κп��� BeginAttack �� ��� ȣ���ϰ� �ɶ�,
	// Combo ������ ������ Ÿ�̹��� �ʹ� ���� EndAttack �� ���� ���� ����.
	virtual bool IsLastCombo() const = 0;

	/* Virtual */
public:
	virtual void CheckAttack(AActor* DoingActor) {}
	virtual void AttackImpulse(AActor* DoingActor, float intensity) {}
	virtual void CheckProcedural(AActor* DoingActor) {}

public:
	// @Onwer - �����ϴ� Pawn (��ü)
	// Protected - OwnerPawn �� �־���.
	// �� �� ���ϰ� Owner �� �����ϱ� ���� ����.
	void SetOwnerPawn(APawn* Owner) { OwnerPawn = Owner; }

protected:
	APawn* OwnerPawn;
};
