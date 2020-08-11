#pragma once

#include "CoreMinimal.h"
#include "Actor/Base/CBaseProjectile.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_Poision.h"

#include "CProjectile_Shuriken.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACProjectile_Shuriken 
	: public ACBaseProjectile
{
	GENERATED_BODY()
	
#pragma	region Reflection
private:
	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class USkeletalMeshComponent* SkeletalMesh;

	//UPROPERTY(VisibleAnywhere, Category = "DamageType")
	//	UCDamageType_Normal*	DT_Normal;

	//UPROPERTY(VisibleAnywhere, Category = "DamageType")
	//	UCDamageType_Poision*	DT_Poision;

	UPROPERTY(VisibleAnywhere, Category = "System")
		class UParticleSystem* P_ExplosionShuriken;

#pragma endregion

protected:
	/* Virtual Overlap */
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	/* Virtual Overlap */
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	ACProjectile_Shuriken();

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
	float DeathTime = 2.0f;
};
