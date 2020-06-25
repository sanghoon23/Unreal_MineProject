#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"

#include "CPL_MageBaseAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MageBaseAttack
	: public UC_BaseAttackState
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

protected:
	UPROPERTY(VisibleAnywhere, Category = "System")
		class UCS_AttackDecision* AttackDecision;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
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
protected:
	class ACPlayer*		Player;

	//@UI
	APlayerController* PlayerController;
	class AHUD_Main* MainHUD;

	// @���ݹ���, Target�� �� ���� �ȿ� ���;����� ���ݰ���
	float AttackRange = 200.0f;

private:
	//@Tick Entry Value
	bool bInputAttackCall = false;

	#pragma endregion
};
