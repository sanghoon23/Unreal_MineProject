#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_BasicBaseAttack.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCHM_BasicBaseAttack 
	: public UActorComponent, public IIC_BaseAttack
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	#pragma endregion

public:	
	UCHM_BasicBaseAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack(AActor * DoingActor) override;
	virtual void OnComboSet(AActor * DoingActor) override;

	virtual void CheckAttack(AActor* DoingActor) override;
	virtual void AttackImpulse(AActor* DoingActor, float intensity) override;
	virtual void CheckProcedural(AActor* DoingActor) override;

#pragma	region Member
public:
	virtual bool GetAttacking() const override { return bAttacking; }
	virtual void SetAttacking(bool bValue) override { bAttacking = bValue; };

	virtual bool GetAttackMode() const override { return bAttackMode; }
	virtual void SetAttackMode(bool bValue) override { bAttackMode = bValue; };

	virtual bool GetComboCheck() const override { return bComboCheck; }
	virtual bool IsLastCombo() const override { return false; }

protected:
	UINT				CurrentComboNum = 0;
	UINT				MaxComboNum = 0;

	bool bAttackMode = false;

	bool bAttacking = false;
	bool bComboCheck = false;

#pragma endregion
};
