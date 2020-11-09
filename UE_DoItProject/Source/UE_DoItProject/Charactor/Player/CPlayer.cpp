#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"
#include "Camera/CameraComponent.h"

#include "WidgetComponent.h"

#include "_GameInst/CGameInst.h"
#include "_GameMode/CBaseGameMode.h"
#include "_GameMode/MyPlayerState.h"
#include "System/CS_MouseController.h"
#include "System/CS_TargetingSystem.h"
#include "Component/CPL_StateMachine.h"
#include "Component/Player/CPL_EquipComp.h"
#include "Component/CPL_ActionInteractSystem.h"
#include "Component/CInverseKinematics.h"
#include "Component/Player/CPL_BlendCameraComp.h"
#include "Component/CMeshParticleComp.h"
#include "Component/Base/C_BaseAbilityComp.h"
#include "Component/Player/CPL_HitComp.h"

#include "Actor/Cable/CPL_CableObject.h"
#include "UI/Widget/WG_FloatingCombo.h"

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
		GetCharacterMovement()->MaxWalkSpeed = OriginMaxSpeed;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	}

	//// Setting Charactor Mesh
	//{
	//	//@Mesh Overlap ����� �׾��� ���� ���� ����
	//	GetMesh()->SetGenerateOverlapEvents(true);
	//}

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

#pragma endregion

	//@UI
	{
		strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_FloatingCombo.BpCWG_FloatingCombo_C'";
		ConstructorHelpers::FClassFinder<UWG_FloatingCombo> LoadFloatingComboUIClass(*strPath);
		if (LoadFloatingComboUIClass.Succeeded())
		{
			FloatingComboClass = LoadFloatingComboUIClass.Class;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	#pragma region Create Other Component / Difference To Charactor
	// Create Component
	{
		TargetSystem			= CreateDefaultSubobject<UCS_TargetingSystem>("TargetingSystem");
		BlendCameraComp			= CreateDefaultSubobject<UCPL_BlendCameraComp>("BlendCameraComp");
		IneverseKinematics		= CreateDefaultSubobject<UCInverseKinematics>("IKComp");
		HitComp					= CreateDefaultSubobject<UCPL_HitComp>("HitComp");
		EquipComp				= CreateDefaultSubobject<UCPL_EquipComp>("PlayerEquipComp");
		InteractSystem			= CreateDefaultSubobject<UCPL_ActionInteractSystem>("InteractSystem");
		MeshParticleComp		= CreateDefaultSubobject<UCMeshParticleComp>("MeshParticleComp");
		AbilityComponent		= CreateDefaultSubobject<UC_BaseAbilityComp>("AbilityComp");

		MouseControl			= CreateDefaultSubobject<UCS_MouseController>(TEXT("MouseControllerComp"));

		StateManager			= CreateDefaultSubobject<UCPL_StateMachine>("PlayerStateManager");
	}
	#pragma endregion

	#pragma region Player Info Setting

	//@1029_PlayerState �� �ű�
	//# ���� ü�� ���·� �������־�� ��.
	//Info.MaxHP = 1000.0f;
	//Info.CurrentHP = 1000.0f;

	//Info.MaxMP = 100.0f;
	//Info.CurrentMP = 50.0f;

	//Info.Name = FName(L"PlayerName");
	//Info.InfoConditionDataArray.Init(nullptr, 5);

	#pragma endregion
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Test Code
	//AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GetWorld());
	//if (GameModeBase != nullptr)
	//{
	//	ACBaseGameMode* MyGameMode = Cast<ACBaseGameMode>(GameModeBase);
	//	if (MyGameMode != nullptr)
	//	{
	//		CLog::Print(L"Call GameMode Succeed!!");
	//	}
	//}

	if (MouseControl == nullptr)
	{
		CLog::Print(L"MouseController NULL!!");
	}

	#pragma region Player Setting
	//Player Setting
	{
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
			bCanMove = true;
			OnGravity();
			bBlockAction = false; //@OFF Block 'Action'
			OffBlockKeyInput();	//@OFF Block 'KeyInput'
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

	//CLog::Print(L"Load And BeginPlay");

	AMyPlayerState* PSta = GetPlayerState<AMyPlayerState>();
	if (PSta != nullptr)
	{
		GetGameInstance<UCGameInst>()->GetPlayerInfoFromId(Info, PSta->PlayerId);
		CLog::Print(L"GetPlayerInfo!!");
		CLog::Print(PSta->PlayerId);
	}
}

// #Edit * 0219
// Montage ���� ���ο� ����, bBlockInput Setting
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@#1104_UGameInst ���, PlayerState
	{
		AMyPlayerState* PState = GetPlayerState<AMyPlayerState>();
		if (PState != nullptr)
		{
			GetGameInstance<UCGameInst>()->SetPlayerInfoFromId(Info, PState->PlayerId);
			//PState->SetPlayerInfo(Info);
		}
	}

	//CLog::Print(GetPlayerState()->GetName());

	// @StateMachine ���� StateType �� �޾ƿ�.
	//CurrentStateType = StateManager->GetCurrentAttackStateType();

	// Test Code
	//InputComponent->bBlockInput = false;
	//APlayerController* TestController = Cast<APlayerController>(GetController());
	//if (TestController != nullptr)
	//{
	//	if (TestController->IsInputKeyDown(EKeys::W))
	//	{
	//		EnableInput(TestController);
	//	}
	//}

	// Test Code
	//FRotator ControllerRot = GetControlRotation();
	//CLog::Print(ControllerRot.ToString());
	//FRotator TestRot = GetSpringArmRotation();
	//CLog::Print(TestRot.ToString());
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
	PlayerInputComponent->BindAction("FourAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnFourAttack);
	PlayerInputComponent->BindAction("FiveAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnFiveAttack);

	PlayerInputComponent->BindAction("PullActorWithCableAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnPullActorWithCableAction);
}

#pragma region Input
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@Custom

/* Player �� KeyInput �� Block -  Move & Action �� ������� �ʵ��� */
void ACPlayer::OnBlockKeyInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		DisableInput(PlayerController);
		//PlayerController->SetIgnoreMoveInput(true);
	}
}

/* Player �� KeyInput Release */
void ACPlayer::OffBlockKeyInput()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		EnableInput(PlayerController);
		//PlayerController->SetIgnoreMoveInput(false);
		//#1019_
		/*
		ex) ���� ��, �̵��� �ȵ����� ī�޶�� ������ �� �ֵ��� �ϱ� ���ؼ�.
		Reset �������. �׷��� ������ ������ �Լ����󺹱� ��Ű��, �� �ٽ� Delegate �� ���Ƽ� 
		PlayerController->SetIgnoreMoveInput(false) �� �ι� �����. ��, 0 �� �����ʰ� - �Ǿ����.
		IgnoreMoveInput = 0 �̾������ MoveInput ���� ���� ��.
		*/
		//#1029_
		//�׳� �̵��� �ൿ�� �����Ű�� ��.
		//PlayerController->ResetIgnoreMoveInput();
	}
}

void ACPlayer::OnBlockAction()
{
	bBlockAction = true;
}

void ACPlayer::OffBlockAction()
{
	bBlockAction = false;
}

void ACPlayer::OnParticleInPlayer()
{
	LeftParticle->SetVisibility(true);
	RightParticle->SetVisibility(true);
}

void ACPlayer::OffParticleInPlayer()
{
	LeftParticle->SetVisibility(false);
	RightParticle->SetVisibility(false);
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

void ACPlayer::OnUsingDecalMouseControl(FVector DecalCircleSize, AActor* StandardTarget, float StandardRange)
{
	MouseControl->OnUsingDecalMouseControl(DecalCircleSize, StandardTarget, StandardRange);
}

void ACPlayer::OffUsingDecalMouseControl()
{
	MouseControl->OffUsingDecalMouseControl();
}

void ACPlayer::OnMoveForward(float Value)
{
	IfFalseRet(bCanMove);

	FRotator rotation = GetControlRotation();
	FRotator temp = FRotator(0, rotation.Yaw, 0);
	FVector forward = FQuat(temp).GetForwardVector();

	/* Value * MultipleInputSpeed */
	AddMovementInput(forward, Value * MultipleInputSpeed);
}

void ACPlayer::OnMoveRight(float Value)
{
	IfFalseRet(bCanMove);

	FRotator rotation = GetControlRotation();
	FRotator temp = FRotator(0, rotation.Yaw, 0);
	FVector right = FQuat(temp).GetRightVector();

	/* Value * MultipleInputSpeed */
	AddMovementInput(right, Value * MultipleInputSpeed);
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
	float Length = SpringArmComp->TargetArmLength += (ZoomSpeed * Value);
	Length = FMath::Clamp(Length, MinWheel, MaxWheel);
	SpringArmComp->TargetArmLength = Length;
	//CLog::Print(SpringArmComp->TargetArmLength);
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
	TargetSystem->OnFindTargets(GetActorLocation(), 3000.0f);
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

	CLog::Print(L"OnBasicAttack IN!!");

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

void ACPlayer::OnFourAttack()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_BaseAttack* SwitchBaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(3);
	// check(SwitchBaseAttack);
	IfNullRet(SwitchBaseAttack);
	{
		// BeginAttack
		SwitchBaseAttack->BeginAttack(this);
	}
}

void ACPlayer::OnFiveAttack()
{
	IfTrueRet(bBlockAction);

	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_BaseAttack* SwitchBaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(4);
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

#pragma endregion

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@UI

void ACPlayer::GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray)
{
	//@HitComp
	TArray<UCBaseConditionType*> ConditionTypes;
	HitComp->GetConditionDatasOutArray(&ConditionTypes);
	for (UCBaseConditionType* ConditionType : ConditionTypes)
	{
		FViewConditionState Insert;
		Insert.TextureUI = ConditionType->GetTextureUI();
		Insert.ColorAndOpacity = ConditionType->ColorAndOpacity;
		Insert.TintSlateColor = FSlateColor(ConditionType->TintColor);
		Insert.ApplyTime = ConditionType->ApplyTime;

		OutArray->Emplace(Insert);
	}

	//@AbilityComp
	TArray<UCBaseAbility*> Abilities;
	AbilityComponent->GetAbilities(Abilities);
	for (UCBaseAbility* Ability : Abilities)
	{
		FViewConditionState Insert;
		Insert.TextureUI = Ability->GetTextureUI();
		Insert.ColorAndOpacity = Ability->ColorAndOpacity;
		Insert.TintSlateColor = FSlateColor(Ability->TintColor);
		Insert.ApplyTime = Ability->GetApplyTimer();

		OutArray->Emplace(Insert);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@Interface

void ACPlayer::OnDeath()
{
	bDeath = true;

	OnDeathDelegate.Broadcast();
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

	//@Montage �� ���� ����
	IfTrueRet(bDontMontagePlay);

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

void ACPlayer::ActorStopAnimMon(class UAnimMontage* Montage)
{
	check(Montage);
	StopAnimMontage(Montage);
}

void ACPlayer::ActorPausedAnimMonResume()
{
	GetMesh()->GetAnimInstance()->Montage_Resume(CurrentMontage);

	//#Edit 0722 - PengMao �� SecondAttack ������ �°� ������ Ǯ���� �ٷ� �����̸� Input �� ���ͼ� �����ִ� ���¿����� ������ �� �ְ� ��.
	// *����, OffBlockKeyInput �� �ش� ���ݿ��� �������ִ� �ɷ�.
	////@Off Block Key Input
	//OffBlockKeyInput();
}

void ACPlayer::ActorAnimMonPause()
{
	GetMesh()->GetAnimInstance()->Montage_Pause(CurrentMontage);

	//#1030_�̰� �� ��������?..
	//@On Block Key Input
	//OnBlockKeyInput();
}

void ACPlayer::OnCollision()
{
	GetCapsuleComponent()->SetCollisionProfileName("Charactor");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACPlayer::OffCollision()
{
	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IfFalseRetResult(CanBeDamaged(), Info.CurrentHP);
	IfTrueRetResult(bDeath == true, Info.CurrentHP);

	UWorld* const World = GetWorld();

	//@UI
	{
		FVector InsertPos = GetActorLocation();

		UWG_FloatingCombo* FloatingComboUI = CreateWidget<UWG_FloatingCombo>(GetWorld(), FloatingComboClass);
		if (FloatingComboUI != nullptr && bUsingFloatingComboUI)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0); //@��ü��.
			if (PC != nullptr)
			{
				FloatingComboUI->SetInitial(PC, InsertPos, EFloatingComboColor::RED);
				FloatingComboUI->SetDisplayDamageValue(DamageAmount);

				FloatingComboUI->AddToViewport();
			}
			else
			{
				bUsingFloatingComboUI = true;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	float InputDamageAmount = DamageAmount;
	if (Info.BarrierAmount > 0.0f) //@Barrier
	{
		if (Info.BarrierAmount > InputDamageAmount) //@�� ������ ������
		{
			Info.BarrierAmount -= InputDamageAmount;
			return Info.CurrentHP;
		}
		else
		{
			Info.BarrierAmount = 0.0f;
			InputDamageAmount -= Info.BarrierAmount;
		}
	}
	if (InputDamageAmount > 0.0f) //@CurrentHP
	{
		Info.CurrentHP -= InputDamageAmount;
	}

	if (Info.CurrentHP <= 0.0f) //@����
	{
		OnDeath();
	}
	return Info.CurrentHP;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

int ACPlayer::GetCurrentAttackStateType()
{
	return static_cast<int>(StateManager->GetCurrentStateType());
}

/* I_Charactor �� CurrentBaseAction Setting */
void ACPlayer::SetCurrentBaseAction(IIC_BaseAction * IBaseAction)
{
	check(IBaseAction);
	CurrentIBaseAction = IBaseAction;
}

/* TargetingSystem ���� Player �� Tab �� ������ ��, ������ Pawn �� ������ */
APawn * ACPlayer::GetFindAttackTarget()
{
	return TargetSystem->GetCurrentFindAttackTarget();
}

void ACPlayer::AddCurrentHP(float fValue)
{
	Info.CurrentHP += fValue;
	Info.CurrentHP = FMath::Clamp(Info.CurrentHP, 0.0f, Info.MaxHP);
}

void ACPlayer::AddCurrentMP(float fValue)
{
	Info.CurrentMP += fValue;
	Info.CurrentMP = FMath::Clamp(Info.CurrentMP, 0.0f, Info.MaxMP);
}

void ACPlayer::AddBarrierAmount(float fValue)
{
	//@���� ���� ����
	float InputBarrierAmount = Info.BarrierAmount + fValue;
	Info.BarrierAmount += fValue;
	//if (InputBarrierAmount >= Info.MaxHP)
	//{
	//	/* Barrier �� MaxHP �� ���� ���� */
	//	InputBarrierAmount = FMath::Clamp(InputBarrierAmount, 0.0f, Info.MaxHP);
	//}
}

void ACPlayer::AddSpeedToOrigin(float fValue)
{
	Info.AddSpeed += fValue;
	GetCharacterMovement()->MaxWalkSpeed = OriginMaxSpeed + Info.AddSpeed;
}

void ACPlayer::AddATK(float fValue)
{
}

void ACPlayer::AddDEF(float fValue)
{
}

IIC_StateManager * ACPlayer::GetIStateManager()
{
	return Cast<IIC_StateManager>(StateManager);
}

IIC_ActionComp * ACPlayer::GetIActionComp()
{
	return StateManager->GetIActionComp();
}

/* ���� CurrentStateType �� �ش��ϴ� Inteface AttackComp �� ������ */
IIC_AttackComp * ACPlayer::GetIAttackComp()
{
	return StateManager->GetIAttackComp();
}

IIC_HitComp * ACPlayer::GetIHitComp()
{
	return Cast<IIC_HitComp>(HitComp);
}

IIC_EquipComp * ACPlayer::GetIEquipComp()
{
	return Cast<IIC_EquipComp>(EquipComp);
}

IIC_MeshParticle * ACPlayer::GetIMeshParticle()
{
	return Cast<IIC_MeshParticle>(MeshParticleComp);
}

IIC_AbilityComp * ACPlayer::GetIAbilityComp()
{
	return Cast<IIC_AbilityComp>(AbilityComponent);
}
