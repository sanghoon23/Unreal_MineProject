#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAction.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_EquipComp.h"
#include "Interface/IC_MeshParticle.h"
#include "Interface/IC_AbilityComp.h"

#include "Interface/IC_InteractActor.h"

#include "IC_Charactor.generated.h"

UENUM(BlueprintType)
enum class ECharactorType : uint8
{
	PLAYER = 0,
	MONSTER = 1,
};

UINTERFACE(MinimalAPI)
class UIC_Charactor : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnActionResetState, AActor*)
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE(FOnCharactorDestroy)

class UE_DOITPROJECT_API IIC_Charactor
{
	GENERATED_BODY()

public:
	// @Action(Key Input 등) 으로 인해 행동이 시작됐을 때, 진행중인 상태값을 되돌리는 Delegate
	// @param AActor - 주체 객체
	FOnActionResetState		OnActionResetState;

	// @Death - 죽음 상태 실행 시작할 때
	FOnDeath				OnDeathDelegate;

	// @CharactorDestroy - 죽음 상태가 완전히 끝나고 사라질 때
	FOnCharactorDestroy		OnCharactorDestroy;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function */
public:
	virtual ECollisionChannel GetCharactorUsingAttackChannel() const = 0;
	virtual ECharactorType GetCharactorType() const = 0;

	virtual bool IsDeath()		= 0;
	virtual void OnDeath()		= 0;

	virtual void CanMove()		= 0;
	virtual void CanNotMove()	= 0;

	virtual void OnGravity()	= 0;
	virtual void OffGravity()	= 0;

	virtual bool IsJumping()	= 0;
	virtual void OffJumping()	= 0;

	// @현재 적용된, 혹은 적용되어졌던 Montage
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const = 0;

	// @Montage - 실행할 Montage
	// @Speed - Montage 속도
	// @bAlways - Montage 실행성 보장
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) = 0;

	// @현재 실행중인 Montage Stop ( CurrentApplyedMontage )
	virtual void ActorStopCurrentAnimMon() = 0;

	// @해당 몽타주 실행중 이면 Montage Stop ( Input Montage )
	virtual void ActorStopAnimMon(class UAnimMontage* Montage) = 0;

	// @현재 실행중인 Montage Pause ( CurrentApplyedMontage )
	virtual void ActorAnimMonPause() = 0;

	// @Pause 된 CurrentApplyMonatge 를 다시 실행
	virtual void ActorPausedAnimMonResume() = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	virtual void OnCollision() {}
	virtual void OffCollision() {}

	virtual void OffEvade() {} // 회피 해제
	virtual float GetEvadeSpeed() { return 0.0f; } // 회피 동작의 ActorLocation 속도
	virtual FVector GetEvadeDirection() { return FVector(1.f, 0.0f, 0.0f); } // 회피방향

public:
	virtual IIC_StateManager* GetIStateManager() { return nullptr; }
	virtual IIC_AttackComp* GetIAttackComp() { return nullptr; }
	virtual IIC_HitComp* GetIHitComp() { return nullptr; }
	virtual IIC_EquipComp* GetIEquipComp() { return nullptr; }
	virtual IIC_AbilityComp* GetIAbilityComp() { return nullptr; }
	virtual IIC_MeshParticle* GetIMeshParticle() { return nullptr; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Function */
public :
	const bool IsDontMontagePlay() const { return bDontMontagePlay; }

	// @몽타주를 받지 않음 
	//- Ex) 빙결 상황일 땐 다른 공격이 들어와도 실행되지 않아야 함
	void SetDontMontagePlay(bool bValue) { bDontMontagePlay = bValue; }

	/* 현재 동작할 BaseAction */
	IIC_BaseAction* GetCurrentIBaseAction() { return CurrentIBaseAction; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Member */
protected:
	//@Montage 실행시킴 여부
	bool bDontMontagePlay = false;

	//@현재 실행되는 액션 - 'E'
	IIC_BaseAction* CurrentIBaseAction;
};
