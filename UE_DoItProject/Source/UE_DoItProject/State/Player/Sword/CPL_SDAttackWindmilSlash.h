#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"

#include "DamageType/CDamageType_Air.h"

#include "CPL_SDAttackWindmilSlash.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackWindmilSlash
	: public UCPL_SwordBaseAttack
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Air*	DT_Air;

	#pragma endregion

public:	
	UCPL_SDAttackWindmilSlash();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function - IC_BaseAttack */
public:
	void BeginAttack(AActor * DoingActor) override;
	void EndAttack() override;
	//void OnComboSet(AActor * DoingActor) override;
	//bool IsLastCombo() const override;

	void AttackOtherPawn() override;
	void ImpulseAttack(float intensity) override;
	void CheckProcedural() override;

	#pragma region Member
private:
	float AttackRadius = 500.0f;

	#pragma endregion

};
