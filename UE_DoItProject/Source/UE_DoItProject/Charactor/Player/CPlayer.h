#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_EquipComp.h"
#include "Interface/IC_BaseAttack.h"

#include "Component/CPL_StateMachine.h"
#include "Component/Player/CPL_EquipComp.h"

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
		class UCPL_StateMachine* StateMachine;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_EquipComp* EquipComponent;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* LeftParticle;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* RightParticle;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* CurrentMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> HitMontages;

	#pragma endregion

public:
	ACPlayer();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function */
public:
	virtual bool IsDeath() override { return bDeath; }
	virtual void CanMove() override { bCanMove = true; } // 이동 가능.
	virtual void CanNotMove() override { bCanMove = false; } // 이동 불가.
	virtual int GetCurrentStateType() const override { return static_cast<int>(CurrentStateType); }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;

	/* Virtual */
public:
	virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) override;
	
	virtual void OnEvade() override;
	virtual void OffEvade() override;
	virtual float GetEvadeSpeed() { return EvadeSpeed; } // 회피
	virtual FVector GetEvadeDirection() override { return EvadeDirection; } // 회피 방향

public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; }
	virtual IIC_StateManager* GetIStateManager() override { return Cast<IIC_StateManager>(StateMachine); }
	virtual IIC_AttackComp* GetIAttackComp() override;
	virtual IIC_EquipComp* GetIEquipComp() { return Cast<IIC_EquipComp>(EquipComponent); }

	/* Function */
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
	void OnSwapState();
	void OnBasicAttack();

	#pragma	region Member
public:
	// Move
	bool GetCanMove() const { return bCanMove; }

	// Evade
	bool GetEvade() const { return bEvade; }
	void SetEvadeDirection(FVector Direction) { EvadeDirection = Direction; }
	void SetEvadeSpeed(float Speed) { EvadeSpeed = Speed; }

private:
	bool bDeath = false;

	// State
	PlayerStateType CurrentStateType	= PlayerStateType::MAGE;
	bool bChangeStateSwap				= false;

	// Move
	bool bCanMove		= true;
	bool bAxisTurn		= false;

	// Evade
	bool bEvade			= false;
	float EvadeSpeed	= 15.0f;
	FVector EvadeDirection;

	// Ability
	float Health		= 30.0f;

	#pragma endregion

};
