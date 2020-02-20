#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHumanoidMonster.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHumanoidMonster : public ACharacter
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	UPROPERTY(EditAnywhere, Category = "AI")
		TSubclassOf<class ACAIC_HM_Basic> MonAIControllerClass;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RunSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* CurrentMontage;

	#pragma endregion

public:
	ACHumanoidMonster();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#pragma	region Member
public:
	float GetHeath() const { return Health; }

	bool GetDeath() const { return bDeath; }
	void SetDeath(bool bValue) { bDeath = bValue; }

protected:
	float Health = 30.0f;
	bool bDeath = false;

	bool bCanMove = true;

	#pragma endregion

};
