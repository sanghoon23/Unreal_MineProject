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
	virtual void BeginAttack(AActor* Actor) {}
	virtual void EndAttack(AActor* Actor) {}
	virtual void OnComboSet(AActor* Actor) {}

	virtual void CheckAttack(AActor* Actor) {}
	virtual void AttackImpulse(AActor* Actor, float intensity) {}
	virtual void CheckProcedural(AActor* Actor) {}

	virtual bool GetAttacking() { return false; }
	virtual void SetAttacking(bool bValue) {};

	virtual bool GetAttackMode() { return false; }
	virtual void SetAttackMode(bool bValue) {};

	virtual bool GetComboCheck() { return false; }

public:
	void SetOwnerPawn(APawn* Owner) { OwnerPawn = Owner; }

public:
	FDele_BeginAttack	BeginAttackDeleFunc;
	FDele_EndAttack		EndAttackDeleFunc;

protected:
	APawn* OwnerPawn;
};
