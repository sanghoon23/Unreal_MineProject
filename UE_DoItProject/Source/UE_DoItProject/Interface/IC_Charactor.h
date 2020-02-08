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
	// @Action(Key Input ��) ���� ���� �ൿ�� ���۵��� ��, �������� ���°��� �ǵ�����
	FOnActionResetState		OnActionResetState;

	// @Hit ���� ��, ���� ���°����� ������ �ǵ����� Delegate
	FOnHitResetState		OnHitResetState;

	// @Death - ���� ���� ���� ������ ��
	FOnDeath				OnDeath;

	// @CharactorDestroy - ���°� ������ ������ ����� ��
	FOnCharactorDestroy		OnCharactorDestroy;

public:
	virtual bool IsDeath() { return false; }
	virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) {}

	// @Montage - ������ Montage
	// @Speed - Montage �ӵ�
	// @bAlways - Montage ���༺ ����
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) {}

	virtual void CanMove() {} // �̵� ����.
	virtual void CanNotMove() {} // �̵� �Ұ�.
	virtual void OffEvade() {} // ȸ��
	virtual float GetEvadeSpeed() { return 0.0f; } // ȸ�� ������ ActorLocation �ӵ�
	virtual FVector GetEvadeDirection() { return FVector(1.f, 0.0f, 0.0f); } // ȸ�ǹ���

public:
	virtual int GetCurrentStateType() const { return -1; } // @BaseState - StateType(Enum)
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const { return nullptr; } // @���� �����, Ȥ�� ����� ���� �ִϸ��̼�

	virtual IIC_StateManager* GetIStateManager() { return nullptr; }
	virtual IIC_AttackComp* GetIAttackComp() { return nullptr; }
	virtual IIC_MontageComp* GetIMontageComp() { return nullptr; }
	virtual IIC_EquipComp* GetIEquipComp() { return nullptr; }
};
