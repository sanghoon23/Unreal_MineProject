#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/Base/CBaseProjectile.h"

#include "DamageType/CDamageType_Normal.h"

#include "CProjectile_MagicBall.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACProjectile_MagicBall 
	: public ACBaseProjectile
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	DT_Normal;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* ParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "System")
		class UParticleSystem* P_ExplosionMagicBall;

	#pragma endregion

protected:
	/* Virtual Overlap */
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	/* Virtual Overlap */
		virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	
public:	
	ACProjectile_MagicBall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	/* Member */
private:
	bool bSpawned = false;

	float CollisionSphereRadius = 100.0f;

	/* Destory µÉ ½Ã°£ */
	float DeathTime = 5.0f;
};
