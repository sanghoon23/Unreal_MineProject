#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"

#include "CHM_ShamanEnergyPa.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHM_ShamanEnergyPa 
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
		class UParticleSystem* ParticleCasting;

	//UPROPERTY(VisibleAnywhere, Category = "Actor")
	//	/* FireRainActor ���� BeginOverlap ���� Monster Attacking */
	//	class ACParticle_Lighting* LightingActor;

	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DT_Normal;

#pragma endregion

public:
	UCHM_ShamanEnergyPa();
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

	TArray<class UCN_SpawnProjectile*> Notifies_SpawnProjectile;
	
};
