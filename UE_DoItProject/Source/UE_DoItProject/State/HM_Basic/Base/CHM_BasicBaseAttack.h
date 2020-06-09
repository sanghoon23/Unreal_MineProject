#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_BasicBaseAttack.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCHM_BasicBaseAttack 
	: public UC_BaseAttackState
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	#pragma endregion

public:	
	UCHM_BasicBaseAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/* Virtual Function Override */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack() override;
	virtual void OnComboSet(AActor * DoingActor) override;

	virtual void AttackOtherPawn() override;
	virtual void ImpulseAttack(float intensity) override;
	virtual void CheckProcedural() override;

	#pragma	region Member

protected:
	// @���ݹ���, Target�� �� ���� �ȿ� ���;����� ���ݰ��� - Default
	float AttackRange	= 200.0f;

#pragma endregion
};
