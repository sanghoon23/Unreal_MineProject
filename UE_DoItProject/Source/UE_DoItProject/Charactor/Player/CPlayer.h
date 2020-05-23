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
		class UCPL_BlendCameraComp* BlendCameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_StateMachine* StateManager;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_EquipComp* EquipComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* LeftParticle;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UParticleSystemComponent* RightParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCPL_ActionInteractSystem* InteractSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCInverseKinematics* IneverseKinematics;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCPL_TargetingSystem* TargetingSystem;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCS_MouseController* MouseController;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCMeshParticleComp* MeshParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "AttachActor")
		class ACPL_CableObject* CableObject;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* CurrentMontage;
		
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
	virtual void OnDeath() override; //����
	virtual void CanMove() override { bCanMove = true; } // �̵� ����.
	virtual void CanNotMove() override { bCanMove = false; } // �̵� �Ұ�.
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
	virtual const FPlayerInfo GetPlayerInfo() const override { return Info; };

	/* HP �� �÷��ִ� �Լ� */
	virtual void HealthUp(float fValue) override { Info.CurrentHP += fValue; }

	/* Max HP �� �÷��ִ� �Լ� */
	virtual void MaxHealthUp(float fValue) override { Info.MaxHP += fValue; }

	/* Speed �� �÷��ִ� �Լ� */
	virtual void SpeedUp(float fValue) override { Info.Speed += fValue; }

	/* Virtual */
public:
	virtual void OnCollision() override;
	virtual void OffCollision() override;

	virtual void OffEvade() override;
	virtual float GetEvadeSpeed() { return EvadeSpeed; } // ȸ��
	virtual FVector GetEvadeDirection() override { return EvadeDirection; } // ȸ�� ����

public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; }
	virtual IIC_StateManager* GetIStateManager() override;
	virtual IIC_AttackComp* GetIAttackComp() override;
	// TODO : virtual IIC_HitComp* GetIHitComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;
	virtual IIC_MeshParticle* GetIMeshParticle() override;

	/* Function */
public:
	// Block Input
	void OnBlockKeyInput();
	void OffBlockKeyInput();

	// Block Action
	void OnBlockAction()	{ bBlockAction = true; }
	void OffBlockAction()	{ bBlockAction = false; }

	/* LeftHand == 0, RightHand == 1, �� �����Ǿ����� �� �̿��� ���� return*/
	void OnHandIK(uint8 HandNumber);

	/* LeftHand == 0, RightHand == 1, �� �����Ǿ����� �� �̿��� ���� return*/
	void OffHandIK(uint8 HandNumber);

	/*
	(CS_MouseController Class) Mapping
	@param - DecalCircleRange MouseController �ȿ� �ִ� Decal Size ����
	@param - StandardTarget Unproject �� �������� �Ǵ� Target ���� Default = nullptr
	@param - StandardRange �������� �ݰ� Default = 0.0f
	*/
	void OnMouseController(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f);
	void OffMouseController();

	/* Player �ȿ� �ִ� Paritlce �ѱ� */
	void OnParticleInPlayer();

	/* Player �ȿ� �ִ� Paritlce ���� */
	void OffParticleInPlayer();

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
	void OnSwapState();			//@���⽺�� ( �� / ���� )
	void OnLookAround();		//@���ݴ�� ã��

	void OnInteractAction();	//@E Key		- ��ȣ�ۿ�
	void OnBasicAttack();		//@1 Key ����	- �⺻����
	void OnSecondAttack();		//@2 Key ����
	void OnThirdAttack();		//@3 Key ����
	void OnFourAttack();		//@4 Key ����
	void OnFiveAttack();		//@5 Key ����

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
