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
	
protected:
	/* Virtual Overlap */
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	/* Virtual Overlap */
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


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

	/* Member */
private:
	float CollisionSphereRadius = 100.0f;

	/* 생성 여부 */
	bool bSpawned = false;

	/* Destory 될 시간 */
	float DeathTime = 10.0f;
};
