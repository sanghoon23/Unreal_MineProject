#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_SwordBaseAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SwordBaseAttack
	: public UActorComponent, public IIC_BaseAttack
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCPL_SwordBaseAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginAttack(AActor * Actor) override;
	virtual void EndAttack(AActor * Actor) override;
	virtual void OnComboSet(AActor * Actor) override;

	virtual void CheckAttack(AActor* Actor) override;
	virtual void AttackImpulse(AActor* DoingActor, float intensity) override;
	virtual void CheckProcedural(AActor* DoingActor) override;

public:

	#pragma	region Member
public:
	virtual bool GetAttacking() override { return bAttacking; }
	virtual void SetAttacking(bool bValue) override { bAttacking = bValue; };

	virtual bool GetAttackMode() override { return bAttackMode; }
	virtual void SetAttackMode(bool bValue) override { bAttackMode = bValue; };

	virtual bool GetComboCheck() override { return bComboCheck; }

protected:
	UINT				CurrentComboNum = 0;
	UINT				MaxComboNum = 0;

	bool bAttackMode	= false;

	bool bAttacking		= false;
	bool bComboCheck	= false;

#pragma endregion
};
