#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_MontageComp.h"
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

	// @Hit 됐을 때, 기존 상태값들을 원래로 되돌리는 Delegate
	FOnHitResetState		OnHitResetState;

	// @Death - 죽음 상태 실행 시작할 때
	FOnDeath				OnDeath;

	// @CharactorDestroy - 상태가 완전히 끝나고 사라질 때
	FOnCharactorDestroy		OnCharactorDestroy;

public:
	virtual bool IsDeath() { return false; }
	virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) {}

	// @Montage - 실행할 Montage
	// @Speed - Montage 속도
	// @bAlways - Montage 실행성 보장
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) {}

	virtual void CanMove() {} // 이동 가능.
	virtual void CanNotMove() {} // 이동 불가.
	virtual void OffEvade() {} // 회피
	virtual float GetEvadeSpeed() { return 0.0f; } // 회피 동작의 ActorLocation 속도
	virtual FVector GetEvadeDirection() { return FVector(1.f, 0.0f, 0.0f); } // 회피방향

public:
	virtual int GetCurrentStateType() const { return -1; } // @BaseState - StateType(Enum)
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const { return nullptr; } // @현재 적용된, 혹은 적용된 후의 애니메이션

	virtual IIC_StateManager* GetIStateManager() { return nullptr; }
	virtual IIC_AttackComp* GetIAttackComp() { return nullptr; }
	virtual IIC_MontageComp* GetIMontageComp() { return nullptr; }
	virtual IIC_EquipComp* GetIEquipComp() { return nullptr; }
};
