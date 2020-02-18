#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Component/CPL_StateMachine.h"

#include "Component/Player/CPL_EquipComp.h"
#include "_GameController/CPL_TargetingSystem.h"

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
		SpringArmComp->TargetArmLength = 1100.0f;

		SpringArmComp->SocketOffset = FVector(0.f, 0.f, 70.f);

		// Camera
		//CameraCompo->SetWorldLocation(FVector(0, 0, 33));
		CameraComp->bUsePawnControlRotation = true;
		CameraComp->SetRelativeRotation(FRotator(0.0f, -50.0f, 0.f));
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
	// Jump Montage - 임시
	{
		//@Sword
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/SwordMon_Jump.SwordMon_Jump'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> jumpMon(*strPath);
		if (jumpMon.Succeeded())
			JumpMontage = jumpMon.Object;
	}

	// Create Component
	{
		StateManager = CreateDefaultSubobject<UCPL_StateMachine>("PlayerStateManager");
		EquipComp = CreateDefaultSubobject<UCPL_EquipComp>("PlayerEquipComp");
	}

	// Create Targeting System
	TargetingSystem = CreateDefaultSubobject<UCPL_TargetingSystem>("TargetingSystem");
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	// @CurrentStateType - 현재 상태 MAGE
	CurrentStateType = PlayerStateType::MAGE;

	// Initailize Setting
	AddControllerPitchInput(13.f);

	/* Sword Visibility False - 처음 상태는 MAGE 이기 때문, */
	EquipComp->GetDisplayItem(0)->GetStaticMeshComp()->SetVisibility(false);

	// Delegate
	{
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
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Test Code
	//FRotator ControllerRot = GetControlRotation();
	//CLog::Print(ControllerRot.ToString());

	// @StateMachine 에서 StateType 값 받아옴.
	CurrentStateType = StateManager->GetCurrentStateType();
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// @Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayer::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayer::OnLookUp);
	// PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	// @Action
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("DoAxisTurn", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAxisTurn);
	PlayerInputComponent->BindAction("DoAxisTurn", EInputEvent::IE_Released, this, &ACPlayer::OffDoAxisTurn);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("SwapState", EInputEvent::IE_Pressed, this, &ACPlayer::OnSwapState);
	PlayerInputComponent->BindAction("FindTarget", EInputEvent::IE_Pressed, this, &ACPlayer::OnLookAround);

	PlayerInputComponent->BindAction("BasicAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnBasicAttack);
	PlayerInputComponent->BindAction("SecondAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnSecondAttack);
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

	// 캐릭터 회전
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
	// @해당 함수에 IFRet 조건 들어가 있음.
	IIC_BaseAction* BaseAction = StateManager->GetIActionComp()->GetIBaseAction(1);
	if (BaseAction != nullptr)
	{
		BaseAction->OnAction(this);
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

/* @도주기 실행 */
void ACPlayer::OnEvade()
{
	// @해당 함수에 IFRet 조건 들어가 있음.
	IIC_BaseAction* BaseAction = StateManager->GetIActionComp()->GetIBaseAction(0);
	if (BaseAction != nullptr)
	{
		BaseAction->OnAction(this);
	}
}

/* @도주기 끝남 - Notify 에서 호출 */
void ACPlayer::OffEvade()
{
	bEvade = false;
}

/* @Player 공격 상태를 바꾸는 함수 */
void ACPlayer::OnSwapState()
{
	// @해당 함수에 IFRet 조건 들어가 있음.
	StateManager->OnSwapState();
}

/* @Player 위치 반경으로 공격 대상 찾기 */
void ACPlayer::OnLookAround()
{
	TargetingSystem->OnFindTargets();
}

void ACPlayer::OnBasicAttack()
{
	// @해당 함수에 IFRet 조건 들어가 있음.
	IIC_BaseAttack* BaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(0);
	if (BaseAttack != nullptr)
	{
		BaseAttack->BeginAttack(this);
	}
}

void ACPlayer::OnSecondAttack()
{
	// @해당 함수에 IFRet 조건 들어가 있음.
	IIC_BaseAttack* BaseAttack = StateManager->GetIAttackComp()->SetAttackTypeRetIBaseAttack(1);
	if (BaseAttack != nullptr)
	{
		BaseAttack->BeginAttack(this);
	}
}

void ACPlayer::OnGravity()
{
	GetCharacterMovement()->GravityScale = 1.0f;
}

void ACPlayer::OffGravity()
{
	GetCharacterMovement()->GravityScale = 0.0f;
}

void ACPlayer::ActorAnimMonPlay(UAnimMontage * Montage, float Speed, bool bAlways)
{
	check(Montage);

	if (bAlways == false)
	{
		// 만약 캐릭터가 몽타주 실행 중이라면,
		// RetTrue(bMontagePlaying);
		IfTrueRet(GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage));
	}

	PlayAnimMontage(Montage, Speed);
	CurrentMontage = Montage;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

/* TargetingSystem 에서 Player 가 Tab 을 눌렀을 때, 설정된 Pawn 을 가져옴 */
APawn * ACPlayer::GetFindAttackTarget()
{
	return TargetingSystem->GetCurrentFindAttackTarget();
}

IIC_StateManager * ACPlayer::GetIStateManager()
{
	return Cast<IIC_StateManager>(StateManager);
}

/* 현재 CurrentStateType 에 해당하는 Inteface AttackComp 를 가져옴 */
IIC_AttackComp * ACPlayer::GetIAttackComp()
{
	return StateManager->GetIAttackComp();
}

IIC_EquipComp * ACPlayer::GetIEquipComp()
{
	return Cast<IIC_EquipComp>(EquipComp);
}

