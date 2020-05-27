#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_EquipComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CPlayer.generated.h"

enum class EPlayerStateType : uint8;

UCLASS()
class UE_DOITPROJECT_API ACPlayer 
	: public ACharacter, public IIC_Player, public IIC_Charactor
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(EditDefaultsOnly, Category = "Movements")
		float OriginMaxSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, Category = "Movements")
		float MultipleInputSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		float ZoomSpeed = 20.0f;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* LeftParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* RightParticle;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_BlendCameraComp* BlendCameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_StateMachine* StateManager;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_EquipComp* EquipComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCPL_ActionInteractSystem* InteractSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCInverseKinematics* IneverseKinematics;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCMeshParticleComp* MeshParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_AbilityComp* AbilityComp;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCPL_TargetingSystem* TargetingSystem;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCS_MouseController* MouseController;

	UPROPERTY(VisibleAnywhere, Category = "AttachActor")
		class ACPL_CableObject* CableObject;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* CurrentMontage;

	// Test Code
	//UPROPERTY(Category = MapsAndSets, EditAnywhere)
	//	TMap<int32, FString> FruitMap;
	//UPROPERTY(Category = "Asset")
	//	TMultiMap<int32, FString> AttributeMultiMap;

	#pragma endregion

public:
	ACPlayer();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - (IIC_Charactor) */
public:
	virtual bool IsDeath() override { return bDeath; }
	virtual void OnDeath() override; //죽음
	virtual void CanMove() override { bCanMove = true; } // 이동 가능.
	virtual void CanNotMove() override { bCanMove = false; } // 이동 불가.
	virtual void OnGravity() override;
	virtual void OffGravity() override;
	virtual bool IsJumping() override { return bJumping; };
	virtual void OffJumping() override { bJumping = false; }

	virtual int GetCurrentStateType() const override { return static_cast<int>(CurrentStateType); }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;
	virtual void ActorStopCurrentAnimMon() override;
	virtual void ActorStopAnimMon(class UAnimMontage* Montage) override;
	virtual void ActorPausedAnimMonResume() override;
	virtual void ActorAnimMonPause() override;

/* Pure Virtual Function - (IIC_Player) */
public:
	/* Player 안에 기본적 내장되어있는 Paritlce 켜기 */
	virtual void OnParticleInPlayer() override;

	/* Player 안에 기본적 내장되어있는 Paritlce 끄기 */
	virtual void OffParticleInPlayer() override;

	/* */
	virtual const FPlayerInfo& GetPlayerInfo() const override { return Info; };

	/* 능력치 상승 */
	//virtual void AddAbility(EAddAttribute KeyType, const FPlayerAddAbility& Ability) override;

/* Virtual - (IC_Charactor) */
public:
	virtual void OnCollision() override;
	virtual void OffCollision() override;

	virtual void OffEvade() override;
	virtual float GetEvadeSpeed() { return EvadeSpeed; } // 회피
	virtual FVector GetEvadeDirection() override { return EvadeDirection; } // 회피 방향

public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; }
	virtual IIC_StateManager* GetIStateManager() override;
	virtual IIC_AttackComp* GetIAttackComp() override;
	// TODO : virtual IIC_HitComp* GetIHitComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;
	virtual IIC_MeshParticle* GetIMeshParticle() override;

/* Virtual - (IC_Player) */
public:
	virtual IIC_AbilityComp* GetIAbilityComp();

	/* Function */
public:
	// Block Input
	void OnBlockKeyInput();
	void OffBlockKeyInput();

	// Block Action
	void OnBlockAction()	{ bBlockAction = true; }
	void OffBlockAction()	{ bBlockAction = false; }

	/* LeftHand == 0, RightHand == 1, 로 설정되어있음 그 이외의 값은 return*/
	void OnHandIK(uint8 HandNumber);

	/* LeftHand == 0, RightHand == 1, 로 설정되어있음 그 이외의 값은 return*/
	void OffHandIK(uint8 HandNumber);

	/*
	(CS_MouseController Class) Mapping
	@param - DecalCircleRange MouseController 안에 있는 Decal Size 지정
	@param - StandardTarget Unproject 의 기준점이 되는 Target 설정 Default = nullptr
	@param - StandardRange 기준점의 반경 Default = 0.0f
	*/
	void OnMouseController(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f);
	void OffMouseController();

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
	void OnSwapState();			//@무기스왑 ( 검 / 마법 )
	void OnLookAround();		//@공격대상 찾기

	void OnInteractAction();	//@E Key		- 상호작용
	void OnBasicAttack();		//@1 Key 공격	- 기본공격
	void OnSecondAttack();		//@2 Key 공격
	void OnThirdAttack();		//@3 Key 공격
	void OnFourAttack();		//@4 Key 공격
	void OnFiveAttack();		//@5 Key 공격

	void OnPullActorWithCableAction(); //@F Key

	#pragma	region Member
public:
	// @Interface Value
	void SetCurrentBaseAction(IIC_BaseAction* IBaseAction);

	// @TargetSystem
	APawn* GetFindAttackTarget();

	// @CS MouseController
	class UCS_MouseController* GetPlayerCSMouseController() { return MouseController; }

	// @Blend Camera
	class UCPL_BlendCameraComp* GetBlendCameraComp() { return BlendCameraComp; }

	// @Cable Object
	class ACPL_CableObject* GetCableObject() { return CableObject; }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Info
	void AddCurrentHP(float fValue);
	void AddCurrentMP(float fValue);
	void AddBarrierAmount(float fValue);
	void AddSpeedToOrigin(float fValue);
	void AddATK(float fValue);
	void AddDEF(float fValue);

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
	// Info
	FPlayerInfo Info;

	bool bDeath				= false;

	// Block
	bool bBlockAction		= false;

	// State
	EPlayerStateType CurrentStateType;
	bool bChangeStateSwap	= false;

	// Move
	bool bCanMove			= true;
	bool bAxisTurn			= false;

	// Jump
	bool bJumping			= false;

	// Evade
	bool bEvade				= false;
	float EvadeSpeed		= 15.0f; //@Default
	FVector EvadeDirection;

	// Attack
	IIC_BaseAttack* CurrentBaseAttack = nullptr;

	#pragma endregion

};
