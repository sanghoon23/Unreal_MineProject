#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"

#include "CHM_AssaFirstAttack.generated.h"

UENUM()
enum class EHM_AssaFirstComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_AssaFirstAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

#pragma endregion

public:
	UCHM_AssaFirstAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	//
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	//virtual bool IsLastCombo() const override;

	virtual void AttackOtherPawn(UCDamageType_Base* DamageType) override;

#pragma region Member
private:
	class ACHM_Assassin* HM_Assassin;
	class IIC_Charactor* I_Charactor;

	// @공격범위, Target과 이 범위 안에 들어와야지만 공격가능 - Default
	float AttackRange = 200.0f;

	float AttackRadius = 200.0f;

#pragma endregion

};
