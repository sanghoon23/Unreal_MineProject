#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Stun.h"

#include "CHM_ShamanFirstAttack.generated.h"

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

#pragma region Member
private:
	class ACHM_Shaman*	HM_Shaman;

	bool bSkillCasting = false;
	
#pragma endregion
};
