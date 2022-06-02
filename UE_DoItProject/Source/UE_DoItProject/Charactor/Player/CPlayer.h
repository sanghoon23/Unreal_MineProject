#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_WidgetInfo.h"
#include "Interface/IC_StateManager.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_EquipComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CPlayer.generated.h"

enum class EPlayerStateType : uint8;

UCLASS()
class UE_DOITPROJECT_API ACPlayer 
	: public ACharacter, public IIC_Player, public IIC_Charactor, public IIC_WidgetInfo
{
	GENERATED_BODY()

	#pragma	region Reflection
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "_UserID")
		uint8 UserId = 0;



private:
	UPROPERTY(EditDefaultsOnly, Category = "Movements")
		float OriginMaxSpeed = 600.0f;

	UPROPERTY(VisibleAnywhere, Category = "Movements")
		float MultipleInputSpeed = 1.0f;

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

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* P_OnRespawnParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* LeftParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* RightParticle;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_BlendCameraComp* BlendCameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_StateMachine* StateManager;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_HitComp* HitComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCPL_EquipComp* EquipComp;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCPL_ActionInteractSystem* InteractSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UCInverseKinematics* IneverseKinematics;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UCMeshParticleComp* MeshParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		class UC_BaseAbilityComp* AbilityComponent;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCS_TargetingSystem* TargetSystem;

	UPROPERTY(VisibleAnywhere, Category = "Controller")
		class UCS_MouseController* MouseControl;

	UPROPERTY(VisibleAnywhere, Category = "AttachActor")
		class ACPL_CableObject* CableObject;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* CurrentMontage;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		TSubclassOf<class UWG_FloatingCombo> FloatingComboClass;

	//@value
	//.....
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bDeath = false;

	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bBlockAction = false;

	// State
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bChangeStateSwap = false;

	// Condition - #220425
	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bLimitCondition = false;

	// Move
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bCanMove = true;
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bAxisTurn = false;

	// Jump
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bJumping = false;

	// Evade
	UPROPERTY(VisibleAnywhere, Category = "PlayerData")
	bool bEvade = false;

	#pragma endregion

public:
	ACPlayer();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function In Interface - (IC_WidgetInfo) */
	virtual void GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray) override;


	/* Pure Virtual Function - (IIC_Charactor) */
public:
	virtual ECollisionChannel GetCharactorUsingAttackChannel() const override
	{
		return ECollisionChannel::ECC_GameTraceChannel2; 
	}
	virtual ECharactorType GetCharactorType() const override { return CharactorType; }

	virtual void OnInit() override;
	virtual bool IsDeath() override { return bDeath; }
	virtual void OnDeath() override; //����
	virtual void CanMove() override { bCanMove = true; } // �̵� ����.
	virtual void CanNotMove() override { bCanMove = false; } // �̵� �Ұ�.
	virtual void OnGravity() override;
	virtual void OffGravity() override;
	virtual bool IsJumping() override { return bJumping; };
	virtual void OffJumping() override { bJumping = false; }

	virtual void ActorAnimMonPlay(class UAnimMontage* Montage, float Speed, bool bAlways) override;
	virtual void ActorStopCurrentAnimMon() override;
	virtual void ActorStopAnimMon(class UAnimMontage* Montage) override;
	virtual void ActorPausedAnimMonResume() override;
	virtual void ActorAnimMonPause() override;

	virtual void CountingLimitCondition(bool bCount) { (bCount) ? ++LimitConditionNum : --LimitConditionNum; }
	virtual int GetLimitConditionNum() const { return LimitConditionNum; }

/* Pure Virtual Function - (IIC_Player) */
public:
	virtual IIC_StateManager* GetIStateManager() override;

	// Block Input
	virtual void OnBlockKeyInput() override;
	virtual void OffBlockKeyInput() override;

	// Block Action
	virtual void OnBlockAction() override;
	virtual void OffBlockAction()override;

	/* Player �ȿ� �⺻�� ����Ǿ��ִ� Paritlce �ѱ� */
	virtual void OnParticleInPlayer() override;

	/* Player �ȿ� �⺻�� ����Ǿ��ִ� Paritlce ���� */
	virtual void OffParticleInPlayer() override;

	/* */
	virtual const FPlayerInfo& GetPlayerInfo() const override { return Info; };

/* Virtual - (IC_Charactor) */
public:
	virtual void OnCollision() override;
	virtual void OffCollision() override;

	virtual void OffEvade() override;
	virtual FVector GetEvadeDirection() override { return EvadeDirection; } // ȸ�� ����

public:
	virtual const class UAnimMontage* GetCurrentApplyedMontage() const override { return CurrentMontage; }
	virtual IIC_ActionComp* GetIActionComp() override;
	virtual IIC_AttackComp* GetIAttackComp() override;
	virtual IIC_HitComp* GetIHitComp() override;
	virtual IIC_EquipComp* GetIEquipComp() override;
	virtual IIC_MeshParticle* GetIMeshParticle() override;
	virtual IIC_AbilityComp* GetIAbilityComp();

	// @Interface Value
	virtual void SetCurrentBaseAction(IIC_BaseAction* IBaseAction) override;

public:
/* Class Virtual */
	//@return float - ���� ���� ü�� - (PlayerInfo.CurrentHP)
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	/* Function */
public:
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
	void OnUsingDecalMouseControl(FVector DecalCircleSize, AActor* StandardTarget = nullptr, float StandardRange = 0.0f);
	void OffUsingDecalMouseControl();

	void OnUseSpeatatorMode(); //@������ ��� ��ȯ.
	void OnRespawn(); //@������

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
	int GetCurrentAttackStateType();

	// @TargetSystem
	virtual APawn* GetFindAttackTarget() override;

	// @Target System
	class UCS_TargetingSystem* GetTargetingSystem() { return TargetSystem; }

	// @CS MouseController
	class UCS_MouseController* GetPlayerCSMouseController() { return MouseControl; }

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

private:
	// Type
	ECharactorType CharactorType = ECharactorType::PLAYER;

	// Info
	FPlayerInfo Info;

	// Evade
	float EvadeSpeed		= 15.0f; //@Default
	FVector EvadeDirection;

	// Attack
	IIC_BaseAttack* CurrentBaseAttack = nullptr;

	#pragma endregion

};
