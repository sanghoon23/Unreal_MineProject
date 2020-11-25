#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Event.h"
#include "CItem_Barrier.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACItem_Barrier 
	: public ACItem_Event
{
	GENERATED_BODY()

private:
	const float RotationValue = 10.0f;
	const float RotationSpeed = 2.0f;

	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Data")
		float UsingAbilityBarrierTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Data")
		float AddBarrierAmount = 30.0f;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* SM_HologramGlow;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* ParticleBarrier;

	UPROPERTY(VisibleAnywhere, Category = "ReturnValue")
		class UParticleSystemComponent* ParticleComp_Barrier;

private:
	UFUNCTION()
		void OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

public:
	ACItem_Barrier();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Virtual Function */
public:
	virtual void ApplyEvent(AActor* EventedActor) override;

private:
	void DelegateAbilityEnd(AActor* AppliedActor);

	/* Member */
public:
	const float GetBarrierAmountValue() const { return AddBarrierAmount; }
	void SetBarrierAmountValue(float fValue) { AddBarrierAmount = fValue; }

private:

};
