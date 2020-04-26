#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "DamageType/CDamageType_Stun.h"

#include "CPL_ActionPullActorWithCable.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_ActionPullActorWithCable 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Stun*	DT_Stun;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* PullReadyMontage;

	#pragma endregion

public:
	UCPL_ActionPullActorWithCable();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void OnAction() override;

public:
	virtual void BeginActionState() override;
	virtual void TickActionState() override;
	virtual void EndActionState() override;

	/* Function */
private:
	void PullingTargetLocation(AActor* PulledTarget);

	/* Member */
private:
	class ACPlayer* Player;
	class ACPL_CableObject* CableObject;
	APawn* Target = nullptr;

	// Value
	float UsingActionRange = 1700.0f;

	FVector PulledDirection = FVector(0.0f);
	float PulledSpeed = 5.0f;
	float PullRange = 200.0f; //@앞까지 당겨질 거리
	float LerpValue = 0.0f;
	bool bNextMontage = false;
};
