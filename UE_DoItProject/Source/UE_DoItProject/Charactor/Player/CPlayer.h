#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_MontageComp.h"
#include "Interface/IC_EquipComp.h"

#include "Component/Player/CPlayerAttackComp.h"
#include "Component/Player/CPlayerMontageComp.h"
#include "Component/Player/CPlayerEquipComp.h"
#include "State/Player/Base/CPL_BaseState.h"

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

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPlayerAttackComp* AttackComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPlayerMontageComp* MontageComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPlayerEquipComp* EquipComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* LeftParticle;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* RightParticle;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* CurrentMontage;

	UPROPERTY(VisibleInstanceOnly, Category = "Montage")
		class UAnimMontage* MageStateCastMontage;

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

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) override;
	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;

	
	virtual void CanMove() { bCanMove = true; } // 이동 가능.
	virtual void CanNotMove() { bCanMove = false; } // 이동 불가.
	virtual void OffEvade() override; // 회피 해제
	virtual float GetEvadeSpeed() { return EvadeSpeed; } // 회피
	virtual FVector GetEvadeDirection() override { return EvadeDirection; } // 회피 방향

public:
	virtual int GetCurrentStateType() const { return static_cast<int>(CurrentStateType); }
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; }
	virtual IIC_AttackComp* GetIAttackComp() override { return Cast<IIC_AttackComp>(AttackComponent); }
	virtual IIC_MontageComp* GetIMontageComp() override { return Cast<IIC_MontageComp>(MontageComponent); }
	virtual IIC_EquipComp* GetIEquipComp() { return Cast<IIC_EquipComp>(EquipComponent); }


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
	void OnEvade();
	void OnSwapState();

	#pragma	region Member
public:
	bool GetEvade() const { return bEvade; }

private:
	// State
	StateType CurrentStateType = StateType::MAGE;
	bool bChangeStateSwap = false;

	// Move
	bool bCanMove	= true;
	bool bAxisTurn	= false;

	// Evade
	bool bEvade		= false;
	float EvadeSpeed = 15.0f;
	FVector EvadeDirection;

	// Ability
	float Health = 30.0f;
	bool bDeath = false;

	#pragma endregion

};
