#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "_GameController/CPL_TargetingSystem.h"
#include "Component/CPL_StateMachine.h"
#include "Component/Player/CPL_EquipComp.h"
#include "Component/CPL_ActionInteractSystem.h"
#include "Component/CInverseKinematics.h"
#include "Component/Player/CPL_BlendCameraComp.h"

#include "Actor/Cable/CPL_CableObject.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	FString strPath = L"";
	// Set Skeletal Mesh
	{
		strPath = L"SkeletalMesh'/Game/_Mine/Mesh/Player/Yin_WindGauntlet.Yin_WindGauntlet'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(*strPath);
		if (skeletalMesh.Succeeded())
			GetMesh()->SkeletalMesh = skeletalMesh.Object;
	}

	// Mage State
	{
		// Left Hand Particle
		LeftParticle = CreateDefaultSubobject<UParticleSystemComponent>("LeftParticle", true);
		LeftParticle->bAutoActivate = true;
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/P_LightingHand.P_LightingHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> leftParticle(*strPath);
		if (leftParticle.Succeeded())
			LeftParticle->SetTemplate(leftParticle.Object);

		LeftParticle->SetupAttachment(GetMesh(), L"hand_l");
		LeftParticle->SetWorldScale3D(FVector(3.0f));

		// Right Hand Particle
		RightParticle = CreateDefaultSubobject<UParticleSystemComponent>("RightParticle", true);
		RightParticle->bAutoActivate = true;
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/P_LightingHand.P_LightingHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> rightParticle(*strPath);
		if (rightParticle.Succeeded())
			RightParticle->SetTemplate(rightParticle.Object);

		RightParticle->SetupAttachment(GetMesh(), L"hand_r");
		RightParticle->SetWorldScale3D(FVector(3.0f));
	}

	#pragma region Setting Player Value
	// Camera
	{
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
		SpringArmComp->SetupAttachment(GetMesh());

		CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
		CameraComp->SetupAttachment(SpringArmComp);
	}

	// Setting CharacterMovement
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	}

	// Setting Camera
	{
		// SpringArm
		SpringArmComp->bUsePawnControlRotation = true;
		SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
		SpringArmComp->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		SpringArmComp->TargetArmLength = 1000.0f;

		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 70.f);

		// Camera
		//CameraCompo->SetWorldLocation(FVector(0, 0, 33));
		CameraComp->bUsePawnControlRotation = true;
		CameraComp->SetRelativeRotation(FRotator(0.0f, -50.0f, 0.f));
	}

	//NiagaraComp_ImageAfter = CreateDefaultSubobject<UNiagaraComponent>("NiaComponent");
	//NiagaraComp_ImageAfter->SetupAttachment(GetMesh());

	#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////

	#pragma region Create Other Component / Difference To Charactor
	// Create Component
	{
		BlendCameraComp			= CreateDefaultSubobject<UCPL_BlendCameraComp>("BlendCameraComp");
		StateManager			= CreateDefaultSubobject<UCPL_StateMachine>("PlayerStateManager");
		EquipComp				= CreateDefaultSubobject<UCPL_EquipComp>("PlayerEquipComp");
		TargetingSystem			= CreateDefaultSubobject<UCPL_TargetingSystem>("TargetingSystem");
		IneverseKinematics		= CreateDefaultSubobject<UCInverseKinematics>("IKComp");
		InteractSystem			= CreateDefaultSubobject<UCPL_ActionInteractSystem>("InteractSystem");
	}
	#pragma endregion

	//strPath = L"NiagaraSystem'/Game/_Mine/UseParticle/Nia/NiaSystem_ImageAfter.NiaSystem_ImageAfter'";
	//ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiaSystem(*strPath);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Component Setting
	//Component Setting
	{
		//@Niagara Component
		//NiagaraComp_ImageAfter->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

		//@WeakPtr
		//TSharedRef<UNiagaraComponent> NiaCompOwner = MakeShared<UNiagaraComponent>();
		//TWeakObjectPtr<class UNiagaraComponent> InsertPtr(NiagaraComp_AfterImage);
		//StateManager->SetNiaComp_Dash(InsertPtr);
	}
	#pragma endregion

	#pragma region Player Setting
	//Player Setting
	{
		// @CurrentStateType - ���� ���� MAGE
		CurrentStateType = EPlayerStateType::MAGE;

		/* ControllerRot - ó�� Controller ��ġ�� �����ϱ� ���ؼ�, */
		AddControllerPitchInput(13.f);

		/* Sword Visibility False - ó�� ���´� MAGE �̱� ����, */
		EquipComp->GetDisplayItem(0)->GetStaticMeshComp()->SetVisibility(false);

		/* �ٸ� ���� Key Input ���� �� ���� ��, AttackCount �� ������ �ʱ� ���ؼ� ���� CurrentBaseAttack �ʱ�ȭ */
		CurrentBaseAttack = StateManager->GetIAttackComp()->GetCurrentIBaseAttack();
	}
	#pragma endregion

	#pragma region Set Attach Actor
	//@Calbe Object Player �� ���̱�
	{
		FTransform transform = FTransform::Identity;
		FActorSpawnParameters params;
		params.Owner = this;

		CableObject = GetWorld()->SpawnActor<ACPL_CableObject>(ACPL_CableObject::StaticClass(), transform, params);
		CableObject->AttachToComponent
		(
			GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
			, *(CableObject->GetAttachName())
		);
	}
	#pragma endregion

	#pragma region Set Delegate
	// Set Delegate
	{
		// @IC_Charactor
		OnActionResetState.AddLambda([&](AActor*)
		{
			bEvade = false;		//@Evade
			bJumping = false;	//@Jumping
		});

		// @'On' MageState
		StateManager->OnMageState.AddLambda([&]()
		{
			LeftParticle->SetVisibility(true); // MageHandEffect
			RightParticle->SetVisibility(true); // MageHandEffect
		});
		// @'Un' MageState
		StateManager->UnMageState.AddLambda([&]()
		{
			LeftParticle->SetVisibility(false); // MageHandEffect
			RightParticle->SetVisibility(false); // MageHandEffect
		});
	}
	#pragma endregion
}

// #Edit * 0219
// Montage ���� ���ο� ����, bBlockInput Setting
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// @StateMachine ���� StateType �� �޾ƿ�.
	CurrentStateType = StateManager->GetCurrentStateType();

	// Test Code
	//InputComponent->bBlockInput = false;
	//APlayerController* TestController = Cast<APlayerController>(GetController());
	//if (TestController != nullptr)
	//{
	//	if (TestController->IsInputKeyDown(EKeys::W))
	//	{
	//		EnableInput(TestController);
	//		CLog::Print(L"KeyDown!!");
	//	}
	//}

	// Test Code
	//FRotator ControllerRot = GetControlRotation();
	//CLog::Print(ControllerRot.ToString());
	// FRotator TestRot = GetSpringArmRotation();
	// CLog::Print(TestRot.ToString());
	//FVector Right = GetActorRightVector();
	//CLog::Print(Right.ToString());
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// @Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayer::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayer::OnLookUp);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	// @Action
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("DoAxisTurn", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAxisTurn);
	PlayerInputComponent->BindAction("DoAxisTurn", EInputEvent::IE_Released, this, &ACPlayer::OffDoAxisTurn);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_DoubleClick, this, &ACPlayer::OnDash);
	PlayerInputComponent->BindAction("SwapState", EInputEvent::IE_Pressed, this, &ACPlayer::OnSwapState);
	PlayerInputComponent->BindAction("FindTarget", EInputEvent::IE_Pressed, this, &ACPlayer::OnLookAround);
	PlayerInputComponent->BindAction("InteractAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnInteractAction);

	PlayerInputComponent->BindAction("BasicAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnBasicAttack);
	PlayerInputComponent->BindAction("SecondAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnSecondAttack);
	PlayerInputComponent->BindAction("ThirdAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnThirdAttack);

	PlayerInputComponent->BindAction("PullActorWithCableAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnPullActorWithCableAction);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@Implementable Event

//void ACPlayer::CallDashNiagaraEffect()
//{
//	CLog::Print(L"Call Implementable Event!!");
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@Custom

/* Player �� KeyInput �� Block -  Move & Action �� ������� �ʵ��� */
void ACPlayer::OnBlockKeyInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		DisableInput(PlayerController);
	}
}

/* Player �� KeyInput Release */
void ACPlayer::OffBlockKeyInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		EnableInput(PlayerController);
	}
}

/* CInverseKinematic HandIK Mapping */
// LeftHand == 0, RightHand == 1
void ACPlayer::OnHandIK(uint8 HandNumber)
{
	IfTrueRet(HandNumber > 1);
	if(HandNumber == 0)
		IneverseKinematics->OnLeftHandIK();
	else if(HandNumber == 1)
		IneverseKinematics->OnRightHandIK();
}

/* CInverseKinematic HandIK Mapping */
// LeftHand == 0, RightHand == 1
void ACPlayer::OffHandIK(uint8 HandNumber)
{
	IfTrueRet(HandNumber > 1);
	if (HandNumber == 0)
		IneverseKinematics->OffLeftHandIK();
	else if (HandNumber == 1)
		IneverseKinematics->OffRightHandIK();
}

void ACPlayer::OnMoveForward(float Value)
{
	IfFalseRet(bCanMove);

	FRotator rotation = GetControlRotation();
	FRotator temp = FRotator(0, rotation.Yaw, 0);
	FVector forward = FQuat(temp).GetForwardVector();

	AddMovementInput(forward, Value);
}

void ACPlayer::OnMoveRight(float Value)
{
	IfFalseRet(bCanMove);

	FRotator rotation = GetControlRotation();
	FRotator temp = FRotator(0, rotation.Yaw, 0);
	FVector right = FQuat(temp).GetRightVector();

	// ĳ���� ȸ��
	AddMovementInput(right, Value);
}

void ACPlayer::OnTurn(float Value)
{
	IfFalseRet(bAxisTurn);
	AddControllerYawInput(Value);
}

void ACPlayer::OnLookUp(float Value)
{
	IfFalseRet(bAxisTurn);
	AddControllerPitchInput(Value);
}

void ACPlayer::OnZoom(float Value)
{
	SpringArmComp->TargetArmLength += (ZoomSpeed * Value);
}

void ACPlayer::OnJump()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_ActionComp* ActionComp = StateManager->GetIActionComp();
	IfNullRet(ActionComp);
	IIC_BaseAction* BaseAction = ActionComp->GetIBaseAction(1);
	IfNullRet(BaseAction);
	if (BaseAction != nullptr)
	{
		BaseAction->OnAction();
	}
}

void ACPlayer::OnDoAxisTurn()
{
	bAxisTurn = true;
}

void ACPlayer::OffDoAxisTurn()
{
	bAxisTurn = false;
}

void ACPlayer::OnEvade()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_ActionComp* ActionComp = StateManager->GetIActionComp();
	IfNullRet(ActionComp);
	IIC_BaseAction* BaseAction = ActionComp->GetIBaseAction(0);
	IfNullRet(BaseAction);
	if (BaseAction != nullptr)
	{
		BaseAction->OnAction();
	}
}

void ACPlayer::OffEvade()
{
	bEvade = false;
}

/* @Warning - Dash ���� Evade Value ��� */
void ACPlayer::OnDash()
{
	IfTrueRet(bBlockAction);
	StateManager->OnDash();
}

/* @Player ���� ���¸� �ٲٴ� �Լ� */
void ACPlayer::OnSwapState()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	StateManager->OnSwapState();
}

/* @Player ��ġ �ݰ����� ���� ��� ã�� */
void ACPlayer::OnLookAround()
{
	TargetingSystem->OnFindTargets();
}

/* @Player ActionKey - E (��ȣ�ۿ��ϱ�) */
void ACPlayer::OnInteractAction()
{
	InteractSystem->InteractInput();
}

// #Edit *0220
// ���ݿ� ���� BindAction �Լ� 
//@Warning - �����ϰ� ���� ����� 
// IBaseAttack Begin ���� ActorAnimMonPlay �� �׻� ����. 
// Ex) Player->ActorAnimMonPlay(SwordAttackMontages[0], 1.8f, true)
void ACPlayer::OnBasicAttack()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_BaseAttack* SwitchBaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(0);
	// check(SwitchBaseAttack);
	IfNullRet(SwitchBaseAttack);
	{
		// BeginAttack
		SwitchBaseAttack->BeginAttack(this);
	}
}

/* Attack ���� */
void ACPlayer::OnSecondAttack()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_BaseAttack* SwitchBaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(1);
	// check(SwitchBaseAttack);
	IfNullRet(SwitchBaseAttack);
	{
		// BeginAttack
		SwitchBaseAttack->BeginAttack(this);
	}
}

/* Attack ���� */
void ACPlayer::OnThirdAttack()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_BaseAttack* SwitchBaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(2);
	// check(SwitchBaseAttack);
	IfNullRet(SwitchBaseAttack);
	{
		// BeginAttack
		SwitchBaseAttack->BeginAttack(this);
	}
}

/* Cable Object �� ����ؼ� Actor �� ������� Action ���� */
void ACPlayer::OnPullActorWithCableAction()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	StateManager->OnPullActorWithCable();
}

void ACPlayer::OnGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ACPlayer::OffGravity()
{
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ACPlayer::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
{
	check(Montage);

	if (bAlways == false)
	{
		// ���� ĳ���Ͱ� ��Ÿ�� ���� ���̶��,
		// RetTrue(bMontagePlaying);
		IfTrueRet(GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage));
	}

	PlayAnimMontage(Montage, Speed);
	CurrentMontage = Montage;
}

/* ���� ����� CurrentMontage Stop */
void ACPlayer::ActorStopCurrentAnimMon()
{
	StopAnimMontage(CurrentMontage);
}

void ACPlayer::OnCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACPlayer::OffCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

/* I_Charactor �� CurrentBaseAction Setting */
void ACPlayer::SetCurrentBaseAction(IIC_BaseAction * IBaseAction)
{
	check(IBaseAction);
	CurrentIBaseAction = IBaseAction;
}

/* TargetingSystem ���� Player �� Tab �� ������ ��, ������ Pawn �� ������ */
APawn * ACPlayer::GetFindAttackTarget()
{
	return TargetingSystem->GetCurrentFindAttackTarget();
}

IIC_StateManager * ACPlayer::GetIStateManager()
{
	return Cast<IIC_StateManager>(StateManager);
}

/* ���� CurrentStateType �� �ش��ϴ� Inteface AttackComp �� ������ */
IIC_AttackComp * ACPlayer::GetIAttackComp()
{
	return StateManager->GetIAttackComp();
}

IIC_EquipComp * ACPlayer::GetIEquipComp()
{
	return Cast<IIC_EquipComp>(EquipComp);
}

