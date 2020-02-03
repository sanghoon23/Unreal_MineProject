#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"

#include "CPlayer.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACPlayer 
	: public ACharacter, public IIC_Charactor
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(EditDefaultsOnly, Category = "Movements")
		float WalkSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movements")
		float RunSpeed = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ZoomSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* CameraComp;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class UCEquipmentComponent* EquipmentComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPlayerAttackComp* AttackComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPlayerMontageComp* MontageComponent;

	UPROPERTY(VisibleInstanceOnly, Category = "Montage")
		class UAnimMontage* CurrentMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> HitMontages;

private:
	//UFUNCTION()
	//void OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//UFUNCTION()
	//void OnEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	#pragma endregion

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

private:
	// Axis Mapping
	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnTurn(float Value);
	void OnLookUp(float Value);
	void OnZoom(float Value);
	void OnJump();

	// Action Mapping
	void OnDoAxisTurn();
	void OffDoAxisTurn();

	#pragma	region Member
private:
	bool bCanMove = true;
	bool bAxisTurn = false;

	#pragma endregion

};
