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
DECLARE_MULTICAST_DELEGATE(FDele_CancelAttack)

class UE_DOITPROJECT_API IIC_BaseAttack
{
	GENERATED_BODY()

public:
	/* ���� '����' �� Notify ���� �ݵǴ� Delegate */
	FDele_BeginAttack	BeginAttackDeleFunc;

	/* ���� '����' �� Notify ���� �ݵǴ� Delegate */
	FDele_EndAttack		EndAttackDeleFunc;

	/* Pure Function */
public:
	// ���� ���� Call 
	// @DoingActor - �����ϴ� Actor(��ü)
	virtual void BeginAttack(AActor* DoingActor) = 0;

	// ���� ���� Call 
	// @DoingActor - �����ϴ� Actor(��ü)
	virtual void EndAttack() = 0;

	// �޺� ���� - Notify ���� ��.
	// @DoingActor - �����ϴ� Actor(��ü)
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

	// ���� �������� bAttackPossible(bool) - �ϴ� Player ���� ����.
	virtual bool IsAttackPossible() const { return false; }

	/* Virtual */
public:
	// Attack ���� �� �ٸ� Pawn �� HitComp-OnHit �� ���ϴ� �Լ�
	// @DoingActor - �����ϴ� Actor(��ü), �ش� Pawn �־��ָ� ��.
	virtual void AttackOtherPawn() {}

	// Attack ���� �� ����� �ִ� �Լ�, @Notify ���� ����
	// @DoingActor - �����ϴ� Actor(��ü), �ش� Pawn �־��ָ� ��.
	// @intensity - ����
	virtual void ImpulseAttack(float intensity) {}

	// Attack ���� �� Mesh ���� �Լ�
	// @DoingActor - �����ϴ� Actor(��ü), �ش� Pawn �־��ָ� ��.
	virtual void CheckProcedural() {}

	#pragma region Member
public:
	// @Onwer - �����ϴ� Pawn (��ü)
	// Protected - OwnerPawn �� �־���.
	// �� �� ���ϰ� Owner �� �����ϱ� ���� ����.
	void SetOwnerPawn(APawn* Owner);

	/* Attack ���� ���� �� ������ '����' */
	FVector GetAttackMoveDir() const { return AttackMoveDir; }

	/* Attack ���� ���� �� ������ '�ӵ�' */
	float GetAttackMoveSpeed() const { return AttackMoveSpeed; }

protected:
	APawn* OwnerPawn;

	// @Attack �� Move ����
	FVector AttackMoveDir;

	//@Charactor Movement �� ���. (Default=1.0f)
	float AttackMoveSpeed = 1.0f;

	#pragma endregion
};
