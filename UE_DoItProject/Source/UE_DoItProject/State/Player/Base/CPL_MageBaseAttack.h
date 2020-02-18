#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_MageBaseAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MageBaseAttack
	: public UActorComponent, public IIC_BaseAttack
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> MageAttackMontages;

	#pragma endregion

public:	
	UCPL_MageBaseAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void EndAttack() override;
	virtual void OnComboSet(AActor * DoingActor) override;

	virtual void AttackOtherPawn() override;
	virtual void ImpulseAttack(float intensity) override;
	virtual void CheckProcedural() override;

	#pragma	region Member
public:
	bool GetAttacking() const override { return bAttacking; }
	void SetAttacking(bool bValue) override { bAttacking = bValue; };

	bool GetAttackMode() const override { return bAttackMode; }
	void SetAttackMode(bool bValue) override { bAttackMode = bValue; };

	bool GetComboCheck() const override { return bComboCheck; }
	bool IsLastCombo() const override { return false; }

protected:
	class ACPlayer*		Player;

	UINT				CurrentComboNum = 0;
	UINT				MaxComboNum = 0;

	bool bAttackMode	= false;

	bool bAttacking		= false;
	bool bComboCheck	= false;

	#pragma endregion
};
