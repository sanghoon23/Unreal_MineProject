#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "DamageType/CDamageType_StrongAttack.h"

#include "CPL_SDAttackBackRange.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackBackRange 
	: public UCPL_SwordBaseAttack
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_Strong;

	#pragma endregion

public:
	UCPL_SDAttackBackRange();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	void BeginAttack(AActor * DoingActor) override;

	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

private:
	void LookAtTarget(AActor* Target);

	#pragma region Member
private:
	float AttackRadius = 100.0f;

	#pragma endregion

};
