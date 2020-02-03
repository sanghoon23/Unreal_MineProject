#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_Charactor.generated.h"

UINTERFACE(MinimalAPI)
class UIC_Charactor : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitResetState, AActor*)
DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE(FOnCharactorDestroy)

class UE_DOITPROJECT_API IIC_Charactor
{
	GENERATED_BODY()

public:
	FOnHitResetState		OnHitResetState;
	FOnDeath				OnDeath;
	FOnCharactorDestroy		OnCharactorDestroy;

public:
	virtual bool IsDeath() { return false; }

	virtual const class UAnimMontage* GetCurrentApplyedMontage() const { return nullptr; }

	// virtual IIC_AttackComp* GetIAttackComp() { return nullptr; }
	// virtual IIC_MontageComp* GetIMontageComp() { return nullptr; }
	virtual class UCEquipmentComponent* GetEquipmentComp() { return nullptr; }


public:
	virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) {}
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) {}
};
