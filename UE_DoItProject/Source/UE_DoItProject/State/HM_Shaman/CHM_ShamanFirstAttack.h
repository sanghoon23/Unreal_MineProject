#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Stun.h"

#include "CHM_ShamanFirstAttack.generated.h"

UENUM()
enum class EHM_ShamanFirstAttackType : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_ShamanFirstAttack 
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Montages")
		FName SectionStartName = "Start";

	UPROPERTY(EditAnywhere, Category = "Montages")
		FName SectionNextActionName = "NextAction";

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* ParticleLightingCast;

	UPROPERTY(VisibleAnywhere, Category = "Actor")
		/* FireRainActor 에서 BeginOverlap 으로 Monster Attacking */
		class ACParticle_Lighting* LightingActor;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DT_Normal;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Stun*	DT_Stun;

#pragma endregion

public:
	UCHM_ShamanFirstAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	//
public:
	void BeginAttack(AActor * DoingActor) override;
	//void OnComboSet(AActor * DoingActor) override;
	//bool IsLastCombo() const override;

	//void AttackOtherPawn() override;

#pragma region Member
private:
	class ACHM_Shaman*	HM_Shaman;

	// @공격범위, Target과 이 범위 안에 들어와야지만 공격가능 - Default
	float AttackRange = 200.0f;

	float AttackRadius = 100.0f;

	bool bSkillCasting = false;
	
};
