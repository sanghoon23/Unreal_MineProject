#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "IC_HitComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_HitComp : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitFunc, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitResetState, AActor*)

class UE_DOITPROJECT_API IIC_HitComp
{
	GENERATED_BODY()

public:
	// - Hit 됐을 때, 실행할 함수 Delegate
	// @param AActor* - 함수를 수행할 Hitted(Hit 당하는 ) 객체
	// FOnHitFunc				OnHitFunc;

	// - Hit 됐을 때, 기존 상태값들을 원래로 되돌리는 Delegate
	// @param AActor* - Hitted( Hit 당하는 ) 객체
	// FOnHitResetState		OnHitResetState;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	// @AttackingActor - 때리는 주체 Actor
	// @DamageType - 데미지 유형 (Ex. Normal, Air...) 등
	// @DamageAmount - 데미지량
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base* const DamageType, float DamageAmount) = 0;


	#pragma region Member
public:
	/* Hit 당했을 때, 움직일 '방향' */
	FVector GetHitDirection() { return HitDirection; }

	/* Hit 당했을 때, 움직일 '방향' */
	void SetHitDirection(FVector Direction) { HitDirection = Direction; }

	/* Hit 당했을 때, 움직일 '속도' */
	float GetHitMoveSpeed() const { return HitMoveSpeed; }

	/* Hit 당했을 때, 움직일 '속도' */
	void SetHitMoveSpeed(float fValue) { HitMoveSpeed = fValue; }

protected:
	FVector HitDirection = FVector(0.0f);
	float HitMoveSpeed = 1.0f; //@Charactor Movement 를 사용. (Default=1.0f)

	#pragma endregion
};
