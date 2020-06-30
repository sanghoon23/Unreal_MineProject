#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Air.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CPL_SDAttackBackRange.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackBackRange 
	: public UCPL_SwordBaseAttack, public IIC_Component
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Air*			DT_Air;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_Strong;

	UFUNCTION()
		void GoBackToTarget(AActor* Target);

	#pragma endregion

public:
	UCPL_SDAttackBackRange();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Function - IC_BaseAttack */
public:
	void BeginAttack(AActor * DoingActor) override;
	void EndAttack() override;

	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

	#pragma region Member
private:
	float AttackRadius = 250.0f;

	FTimerHandle EndSectionTimerHandle;
	FTimerDelegate EndSectionDelegate;

	#pragma endregion

};
