#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Air.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CHM_MaoThirdAttack.generated.h"

//@Class - PengMao ÀÇ WindMil Attack

UENUM()
enum class EHM_MaoThirdComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoThirdAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Air*	DT_AirFirstAttack;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_StrongAttack*	DT_Strong;

#pragma endregion

public:
	UCHM_MaoThirdAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual Function */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	//virtual bool IsLastCombo() const override;

	virtual void AttackOtherPawn() override;


	/* Function */
public:
	void DelBeatedStartTarget(AActor* Subject);
	void DelBeatedEndTaraget(AActor* Subject);

#pragma region Member
private:
	class ACHM_PengMao*	HM_PengMao;
	class IIC_Charactor* I_Charactor;

	float AttackRadius = 400.0f;

	bool bCheckSectionRange = true;
	float StartSectionPlayRate = 0.2f;
	float NextSectionPlayRate = 0.9f;

#pragma endregion
};
