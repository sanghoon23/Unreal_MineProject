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
	// @DoingActor - 공격하는 Actor(주체)
	virtual void BeginAttack(AActor* DoingActor) = 0;
	virtual void EndAttack(AActor* DoingActor) = 0;
	virtual void OnComboSet(AActor* DoingActor) = 0;

	virtual bool GetAttacking() const = 0;
	virtual void SetAttacking(bool bValue) = 0;

	virtual bool GetAttackMode() const = 0;
	virtual void SetAttackMode(bool bValue) = 0;

	virtual bool GetComboCheck() const = 0;

	// @Combo 의 마지막 구간을 정확히 알기 위해서.
	// AI 부분에서 BeginAttack 을 계속 호출하게 될때,
	// Combo 마지막 구간의 타이밍이 너무 빨라서 EndAttack 이 재대로 동작 않음.
	virtual bool IsLastCombo() const = 0;

	/* Virtual */
public:
	virtual void CheckAttack(AActor* DoingActor) {}
	virtual void AttackImpulse(AActor* DoingActor, float intensity) {}
	virtual void CheckProcedural(AActor* DoingActor) {}

public:
	// @Onwer - 공격하는 Pawn (주체)
	// Protected - OwnerPawn 에 넣어짐.
	// 좀 더 편하게 Owner 로 접근하기 위해 만듬.
	void SetOwnerPawn(APawn* Owner) { OwnerPawn = Owner; }

protected:
	APawn* OwnerPawn;
};
