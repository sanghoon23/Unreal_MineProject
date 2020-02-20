#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_EquipComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CPlayer.generated.h"

enum class PlayerStateType : uint8;

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
		class UCPL_StateMachine* StateManager;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_EquipComp* EquipComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* LeftParticle;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* RightParticle;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCPL_TargetingSystem* TargetingSystem;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* CurrentMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		TArray<class UAnimMontage*> HitMontages;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* JumpMontage;

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
	virtual void OnGravity() override;
	virtual void OffGravity() override;
	virtual bool IsJumping() override { return bJumping; };
	virtual void OffJumping() override { bJumping = false; }

	virtual int GetCurrentStateType() const override { return static_cast<int>(CurrentStateType); }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;

	/* Virtual */
public:
	virtual void OffEvade() override;
	virtual float GetEvadeSpeed() { return EvadeSpeed; } // 회피
	virtual FVector GetEvadeDirection() override { return EvadeDirection; } // 회피 방향

public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; }
	virtual IIC_StateManager* GetIStateManager() override;
	virtual IIC_AttackComp* GetIAttackComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;

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
	void OnEvade();
	void OnDash();
	void OnSwapState(); //@무기스왑 ( 검 / 마법 )
	void OnLookAround(); //@공격대상 찾기

	void OnBasicAttack(); // @1번 공격 - 기본공격
	void OnSecondAttack(); // @2번 공격

	#pragma	region Member
public:
	// @TargetSystem
	APawn* GetFindAttackTarget();

	// Move
	bool GetCanMove() const { return bCanMove; }

	// Jump
	void SetJumping(bool bValue) { bJumping = bValue; }

	// Evade
	bool GetEvade() const { return bEvade; }
	void SetEvade(bool bValue) { bEvade = bValue; }
	void SetEvadeDirection(FVector Direction) { EvadeDirection = Direction; }
	void SetEvadeSpeed(float Speed) { EvadeSpeed = Speed; }

private:
	bool bDeath			= false;

	// State
	PlayerStateType CurrentStateType;
	bool bChangeStateSwap	= false;

	// Move
	bool bCanMove		= true;
	bool bAxisTurn		= false;

	// Jump
	bool bJumping		= false;

	// Evade
	bool bEvade			= false;
	float EvadeSpeed	= 15.0f;
	FVector EvadeDirection;

	// Attack
	IIC_BaseAttack* CurrentBaseAttack = nullptr;

	// Ability
	float Health		= 30.0f;

	#pragma endregion

};
