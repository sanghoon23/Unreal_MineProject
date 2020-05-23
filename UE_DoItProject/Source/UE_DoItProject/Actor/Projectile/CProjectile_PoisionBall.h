#pragma once

#include "CoreMinimal.h"
#include "Actor/Base/CBaseProjectile.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Poision.h"

#include "CProjectile_PoisionBall.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACProjectile_PoisionBall 
	: public ACBaseProjectile
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DamageType_Normal;

	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Poision*	DamageType_Poision;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* ParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "System")
		class UParticleSystem* P_ExplosionPoisionBall;

	#pragma endregion
	
protected:
	/* Virtual Overlap */
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	/* Virtual Overlap */
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	ACProjectile_PoisionBall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Vritual */
public:
	virtual void Explosion() override;

	/* Member */
private:
	float CollisionSphereRadius = 100.0f;

	/* 생성 여부 */
	bool bSpawned = false;

	/* Destory 될 시간 */
	float DeathTime = 5.0f;
};
