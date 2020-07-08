#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CDisplayItem.h"
#include "CItemBox_MediumWood.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACItemBox_MediumWood 
	: public ACDisplayItem
{
	GENERATED_BODY()

	const float DeathTimeAfterRunning = 5.0f;

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* OpeningParticle;

	//UPROPERTY(VisibleAnywhere, Category = "Data")
	//	class UAnimMontage* OpeningMontage;

private:
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

public:
	ACItemBox_MediumWood();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	/* Function */
private:
	void SpawnItem();

	/* Member */
public:
	const bool GetBoxOpening() const { return bBoxOpening; }

private:
	bool bBoxOpening = false;
};
