#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
		SpringArmComp->SetRelativeLocation(FVector(0, 0, 140.0f));
		SpringArmComp->SetRelativeRotation(FRotator(0, 90.0f, 0));
		SpringArmComp->TargetArmLength = 1100.0f;

		SpringArmComp->SocketOffset = FVector(0, 40, 50);

		// Camera
		//CameraCompo->SetWorldLocation(FVector(0, 0, 33));
		CameraComp->bUsePawnControlRotation = true;
		CameraComp->SetRelativeRotation(FRotator(-25, -10, 0));
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create Component
	{
		StateMachine = CreateDefaultSubobject<UCPL_StateMachine>("StateMachine");
		EquipComponent = CreateDefaultSubobject<UCPL_EquipComp>("EquipmentComponent");
	}
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	/* Sword Visibility False - ó�� ���´� MAGE �̱� ����, */
	EquipComponent->GetDisplayItem(0)->GetStaticMeshComp()->SetVisibility(false);

	// Delegate
	{
		// @'On' MageState
		StateMachine->OnMageState.AddLambda([&]()
		{
			LeftParticle->SetVisibility(true); // MageHandEffect
			RightParticle->SetVisibility(true); // MageHandEffect
		});
		// @'Un' MageState
		StateMachine->UnMageState.AddLambda([&]()
		{
			LeftParticle->SetVisibility(false); // MageHandEffect
			RightParticle->SetVisibility(false); // MageHandEffect
		});
	}
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// @StateMachine ���� StateType �� �޾ƿ�.
	CurrentStateType = StateMachine->GetCurrentStateType();
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACPlayer::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPlayer::OnLookUp);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPlayer::OnZoom);

	// Action
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("DoAxisTurn", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAxisTurn);
	PlayerInputComponent->BindAction("DoAxisTurn", EInputEvent::IE_Released, this, &ACPlayer::OffDoAxisTurn);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
	PlayerInputComponent->BindAction("SwapState", EInputEvent::IE_Pressed, this, &ACPlayer::OnSwapState);
	PlayerInputComponent->BindAction("BasicAttack", EInputEvent::IE_Pressed, this, &ACPlayer::OnBasicAttack);
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
	IIC_BaseAttack* BaseAttack = StateMachine->GetIAttackComp()->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		IfTrueRet(BaseAttack->GetAttacking()); // @IF TRUE RETURN
	}

	// @IF TRUE RETURN
	IfTrueRet(bEvade); //@Evade Check
	IfTrueRet(EquipComponent->GetEquiping()); //@Equping Check

	// @IF FALSE RETURN
	IfFalseRet(bCanMove);

	Jump();
}

void ACPlayer::OnDoAxisTurn()
{
	bAxisTurn = true;
}

void ACPlayer::OffDoAxisTurn()
{
	bAxisTurn = false;
}

/* @���ֱ� ���� */
void ACPlayer::OnEvade()
{
	// @�ش� �Լ��� IFRet ���� �� ����.
	StateMachine->GetIActionComp()->GetIBaseAction(0)->OnAction(this);
}

/* @���ֱ� ���� - Notify ���� ȣ�� */
void ACPlayer::OffEvade()
{
	bEvade = false;
}

/* @Player ���� ���¸� �ٲٴ� �Լ� */
void ACPlayer::OnSwapState()
{
	// @�ش� �Լ��� IFRet ���� �� ����.
	StateMachine->OnSwapState();
}

void ACPlayer::OnBasicAttack()
{
	// @�ش� �Լ��� IFRet ���� �� ����.
	IIC_BaseAttack* BaseAttack = StateMachine->GetIAttackComp()->SetAttackTypeRetIBaseAttack(0);
	BaseAttack->BeginAttack(this);
}

void ACPlayer::OnHit(AActor * AttackActor, UINT HitAnimNum, float AnimSpeed)
{
	IfTrueRet(bDeath);

	// Delegate ���� - ���°� �ʱ�ȭ.
	OnHitResetState.Broadcast(this);

	FVector Location = GetActorLocation();
	FVector OtherLocation = AttackActor->GetActorLocation();

	// Hit Animation ����
	ActorAnimMonPlay(HitMontages[HitAnimNum], AnimSpeed, true);

	// �켱 ������ �������� ������.
	if (Controller != nullptr)
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Location, OtherLocation);
		GetController()->SetControlRotation(Rotation);
	}
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get, Set

/* ���� CurrentStateType �� �ش��ϴ� Inteface AttackComp �� ������ */
IIC_AttackComp * ACPlayer::GetIAttackComp()
{
	int Type = static_cast<int>(CurrentStateType);
	return StateMachine->GetIAttackComp();
}

