#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CHumanoidMonster.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACHumanoidMonster 
	: public ACharacter
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	/* HP == 0 이 되었을 때, DeathCall 함수 에서 캐릭터가 지워질 시간 */
	UPROPERTY(EditAnywhere, Category = "Movement")
		float DeathCallFunctionTimer = 8.0f; 

	UPROPERTY(EditAnywhere, Category = "Movement")
		float WalkSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RunSpeed = 500.0f;

	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* CurrentMontage;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		TSubclassOf<class UWG_FloatingCombo> FloatingComboClass;

	#pragma endregion

public:
	ACHumanoidMonster();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Pure Virtual Function - (IC_Monster) */
public:


	/* Function */
public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Virtual Monster Info */
	virtual void AddCurrentHP(float fValue);
	virtual void AddCurrentMP(float fValue);
	virtual void AddATK(float fValue);
	virtual void AddDEF(float fValue);

	#pragma	region Member
public:
	bool GetDeath() const { return bDeath; }
	void SetDeath(bool bValue) { bDeath = bValue; }

protected:
	bool bDeath = false;

	bool bCanMove = true;

	#pragma endregion

};
