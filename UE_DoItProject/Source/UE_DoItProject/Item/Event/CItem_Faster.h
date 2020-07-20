#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Event.h"
#include "CItem_Faster.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACItem_Faster 
	: public ACItem_Event
{
	GENERATED_BODY()

private:
	const float RotationValue = 10.0f;
	const float RotationSpeed = 2.0f;
	
	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Data")
		float UsingAbilitySpeedUpperTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Data")
		float AddSpeedValue = 300.0f;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* SM_HologramGlow;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
		class UCPLAbility_SpeedUpper* AbilitySpeedUpper;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* FasterParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SpeedUp_LHand;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* SpeedUp_RHand;

	//UPROPERTY(VisibleAnywhere, Category = "Particle")
	//	class UParticleSystemComponent* ParticleComp_LHand;

	//UPROPERTY(VisibleAnywhere, Category = "Particle")
	//	class UParticleSystemComponent* ParticleComp_RHand;

private:
	UFUNCTION()
		void OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	#pragma endregion

public:
	ACItem_Faster();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Virtual Function */
public:
	virtual void ApplyEvent(AActor* EventedActor) override;

private:
	void DelegateAbilityStart(AActor* AppliedActor);
	void DelegateAbilityEnd(AActor* AppliedActor);

	/* Member */
public:
	const float GetSpeedValue() const { return AddSpeedValue; }
	void SetSpeedValue(float fValue) { AddSpeedValue = fValue; }

private:

	//class UCPLAbility_Speed* AbilitySpeed;
};
