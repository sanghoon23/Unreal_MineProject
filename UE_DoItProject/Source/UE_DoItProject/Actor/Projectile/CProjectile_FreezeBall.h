#pragma once

#include "CoreMinimal.h"
#include "Actor/Base/CBaseProjectile.h"
#include "DamageType/CDamageType_Freeze.h"

#include "CProjectile_FreezeBall.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACProjectile_FreezeBall 
	: public ACBaseProjectile
{
	GENERATED_BODY()

private:
	const float FreezeingTime = 5.0f;

	#pragma	region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Freeze*	DT_Freeze;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* ParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "System")
		class UParticleSystem* P_ExplosionFreezeBall;

	#pragma endregion
	
public:
	ACProjectile_FreezeBall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Vritual */
public:
	virtual void Explosion() override;

	/* Function */
private:
	void FreezeStartDel(AActor* Subject);
	void FreezeEndDel(AActor* Subject);

	void CheckSettingTarget();


	/* Member */
private:
	float CollisionSphereRadius = 50.0f;

	/* ���� ���� */
	bool bSpawned = false;

	/* Destory �� �ð� */
	float DeathTime = 10.0f;
};
