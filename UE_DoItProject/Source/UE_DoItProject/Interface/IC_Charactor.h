#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAction.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_EquipComp.h"
#include "Interface/IC_MeshParticle.h"
#include "Interface/IC_InteractActor.h"

#include "IC_Charactor.generated.h"

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
	// @Action(Key Input ��) ���� ���� �ൿ�� ���۵��� ��, �������� ���°��� �ǵ����� Delegate
	// @param AActor - ��ü ��ü
	FOnActionResetState		OnActionResetState;

	// @Death - ���� ���� ���� ������ ��
	FOnDeath				OnDeathDelegate;

	// @CharactorDestroy - ���� ���°� ������ ������ ����� ��
	FOnCharactorDestroy		OnCharactorDestroy;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function */
public:
	virtual bool IsDeath() = 0;
	virtual void OnDeath() = 0;
	virtual void CanMove() = 0;
	virtual void CanNotMove() = 0;

	virtual void OnGravity() = 0;
	virtual void OffGravity() = 0;

	virtual bool IsJumping() = 0;
	virtual void OffJumping() = 0;

	// @Charactor �� ������� - ( AnimInst ���� ���� )
	virtual int GetCurrentStateType() const = 0;

	// @���� �����, Ȥ�� ����Ǿ����� Montage
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const = 0;

	// @Montage - ������ Montage
	// @Speed - Montage �ӵ�
	// @bAlways - Montage ���༺ ����
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) = 0;

	// @���� �������� Montage Stop ( CurrentApplyedMontage )
	virtual void ActorStopCurrentAnimMon() = 0;

	// @�ش� ��Ÿ�� ������ �̸� Montage Stop ( Input Montage )
	virtual void ActorStopAnimMon(class UAnimMontage* Montage) = 0;

	// @���� �������� Montage Pause ( CurrentApplyedMontage )
	virtual void ActorAnimMonPause() = 0;

	// @Pause �� CurrentApplyMonatge �� �ٽ� ����
	virtual void ActorPausedAnimMonResume() = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	virtual void OnCollision() {}
	virtual void OffCollision() {}

	virtual void OffEvade() {} // ȸ�� ����
	virtual float GetEvadeSpeed() { return 0.0f; } // ȸ�� ������ ActorLocation �ӵ�
	virtual FVector GetEvadeDirection() { return FVector(1.f, 0.0f, 0.0f); } // ȸ�ǹ���

public:
	virtual IIC_StateManager* GetIStateManager() { return nullptr; }
	virtual IIC_AttackComp* GetIAttackComp() { return nullptr; }
	virtual IIC_HitComp* GetIHitComp() { return nullptr; }
	virtual IIC_EquipComp* GetIEquipComp() { return nullptr; }
	virtual IIC_MeshParticle* GetIMeshParticle() { return nullptr; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Function */
public :
	/* ���� ������ BaseAction */
	IIC_BaseAction* GetCurrentIBaseAction() { return CurrentIBaseAction; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Member */
protected:
	IIC_BaseAction* CurrentIBaseAction;
};
