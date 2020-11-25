#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Charactor/Player/CPlayer.h"

#include "CPL_WorldSpectatorCam.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACPL_WorldSpectatorCam 
	: public ACharacter
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Movements")
		float SpectatorCameraSpeed = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ZoomSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float MinWheel = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float MaxWheel = 1200.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* CameraComp;

	#pragma endregion

public:
	ACPL_WorldSpectatorCam();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	void SetRespawnPlayer(ACPlayer* RespawnPlayerPtr);

private:
	void BindRespawnPlayerFunc();

private:
	// Axis Mapping
	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	void OnMoveUpper(float Value);
	void OnTurn(float Value);
	void OnLookUp(float Value);
	void OnZoom(float Value);

private:
	ACPlayer* RespawnPlayer = nullptr;

};
