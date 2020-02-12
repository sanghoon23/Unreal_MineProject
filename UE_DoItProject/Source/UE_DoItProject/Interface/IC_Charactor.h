#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_EquipComp.h"

#include "IC_Charactor.generated.h"

UINTERFACE(MinimalAPI)
class UIC_Charactor : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionResetState, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitResetState, AActor*)
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE(FOnCharactorDestroy)

class UE_DOITPROJECT_API IIC_Charactor
{
	GENERATED_BODY()

public:
	// @Action(Key Input 등) 으로 인해 행동이 시작됐을 때, 진행중인 상태값을 되돌리는
	FOnActionResetState		OnActionResetState;

	// @Death - 죽음 상태 실행 시작할 때
	FOnDeath				OnDeath;

	// @CharactorDestroy - 상태가 완전히 끝나고 사라질 때
	FOnCharactorDestroy		OnCharactorDestroy;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function */
public:
	virtual bool IsDeath() = 0; // 캐릭터의 죽음 여부.
	virtual void CanMove() = 0; // 이동 가능.
	virtual void CanNotMove() = 0;// 이동 불가.

	// @ Charactor 의 현재상태. (AnimInst 에서 쓰임)
	virtual int GetCurrentStateType() const = 0;

	// @Montage - 실행할 Montage
	// @Speed - Montage 속도
	// @bAlways - Montage 실행성 보장
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	virtual void OnEvade() {}
	virtual void OffEvade() {} // 회피 해제
	virtual float GetEvadeSpeed() { return 0.0f; } // 회피 동작의 ActorLocation 속도
	virtual FVector GetEvadeDirection() { return FVector(1.f, 0.0f, 0.0f); } // 회피방향

public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const { return nullptr; } // @현재 적용된, 혹은 적용된 후의 애니메이션
	virtual IIC_StateManager* GetIStateManager() { return nullptr; }
	virtual IIC_AttackComp* GetIAttackComp() { return nullptr; }
	virtual IIC_EquipComp* GetIEquipComp() { return nullptr; }
};
