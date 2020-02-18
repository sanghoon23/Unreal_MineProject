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
	/* 공격 '시작' 시 Notify 에서 콜되는 Delegate */
	FDele_BeginAttack	BeginAttackDeleFunc;

	/* 공격 '종료' 시 Notify 에서 콜되는 Delegate */
	FDele_EndAttack		EndAttackDeleFunc;

	/* Pure Function */
public:
	// 공격 시작 Call 
	// @DoingActor - 공격하는 Actor(주체)
	virtual void BeginAttack(AActor* DoingActor) = 0;

	// 공격 종료 Call 
	// @DoingActor - 공격하는 Actor(주체)
	virtual void EndAttack() = 0;

	// 콤보 연계 - Notify 에서 콜.
	// @DoingActor - 공격하는 Actor(주체)
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

	// 공격 가능한지 bAttackPossible(bool) - 일단 Player 에서 쓰임.
	virtual bool IsAttackPossible() const { return false; }

	/* Virtual */
public:
	// Attack 실행 시 다른 Pawn 의 HitComp-OnHit 을 콜하는 함수
	// @DoingActor - 공격하는 Actor(주체), 해당 Pawn 넣어주면 됨.
	virtual void AttackOtherPawn() {}

	// Attack 실행 시 충격파 주는 함수, @Notify 에서 쓰임
	// @DoingActor - 공격하는 Actor(주체), 해당 Pawn 넣어주면 됨.
	// @intensity - 강도
	virtual void ImpulseAttack(float intensity) {}

	// Attack 실행 시 Mesh 절단 함수
	// @DoingActor - 공격하는 Actor(주체), 해당 Pawn 넣어주면 됨.
	virtual void CheckProcedural() {}

	#pragma region Member
public:
	// @Onwer - 공격하는 Pawn (주체)
	// Protected - OwnerPawn 에 넣어짐.
	// 좀 더 편하게 Owner 로 접근하기 위해 만듬.
	void SetOwnerPawn(APawn* Owner);

	/* Attack 동작 실행 중 움직일 '방향' */
	FVector GetAttackMoveDir() const { return AttackMoveDir; }

	/* Attack 동작 실행 중 움직일 '속도' */
	float GetAttackMoveSpeed() const { return AttackMoveSpeed; }

protected:
	APawn* OwnerPawn;

	// @Attack 시 Move 방향
	FVector AttackMoveDir;

	//@Charactor Movement 를 사용. (Default=1.0f)
	float AttackMoveSpeed = 1.0f;

	#pragma endregion
};
