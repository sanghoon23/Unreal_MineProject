#pragma once

#include "CoreMinimal.h"
#include "Item/Base/CItem_Event.h"
#include "CItem_Barrier.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACItem_Barrier 
	: public ACItem_Event
{
	GENERATED_BODY()
	
	//@Warning -
	/* 
	객체가 사라져 Delegate.AddUObject 부분이 실행될 수 없음을 방지하기 위해 - CItem_Faster.
	캡처 - [&] AddLambda 사용
	CItem_Faster 에서와 같이 DeathTimeAfterRunning 을 적용되는 시간과 같게 두지 않음
	AbilityBarrier 중첩 시, 시간 누적.
	*/

private:
	const float UsingAbilityTime = 5.0f;
	const float DeathTimeAfterRunning = 2.0f;

	const float RotationValue = 10.0f;
	const float RotationSpeed = 2.0f;

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UStaticMeshComponent* SM_HologramGlow;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
		class UCPLAbility_Barrier* AbilityBarrier;

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
	float AddBarrierAmount = 30.0f;

};
