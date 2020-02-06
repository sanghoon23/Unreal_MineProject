#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPL_BaseState.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_SwordBaseAttack.generated.h"

UENUM()
enum class USwordMontageType : uint8 // @SwordMontageType - Sword 상태일 때의 공격 Type
{
	BASIC = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SwordBaseAttack
	: public UCPL_BaseState, public IIC_BaseAttack
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
	USwordMontageType	AttackType = USwordMontageType::END;
	UINT				CurrentComboNum = 0;
	UINT				MaxComboNum = 0;

	bool bAttackMode	= false;

	bool bAttacking		= false;
	bool bComboCheck	= false;

#pragma endregion
};
