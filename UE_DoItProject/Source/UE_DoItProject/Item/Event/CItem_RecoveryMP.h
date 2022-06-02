#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Event.h"
#include "CItem_RecoveryMP.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACItem_RecoveryMP 
	: public ACItem_Event
{
	GENERATED_BODY()
	
private:
	/* 해당 시간은 Healing Particle 시간보다 커야함. */
	//그렇지 않으면, ParticleSystem 이 실행 도중 없어질 수 있음.
	const float DeathTimeAfterRunning = 3.0f;

	const float RotationValue = 10.0f;
	const float RotationSpeed = 2.0f;

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* SM_HologramGlow;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* HealingParticle;

	UPROPERTY(EditAnywhere, Category = "Data")
		float RecoveryMPValue = 10.0f;

private:
	UFUNCTION()
		void OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

public:
	ACItem_RecoveryMP();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Virtual Function */
public:
	virtual void ApplyEvent(AActor* EventedActor) override;

	/* Member */
public:
	const float GetMPValue() const { return RecoveryMPValue; }
	void SetMPValue(float fValue) { RecoveryMPValue = fValue; }

};
