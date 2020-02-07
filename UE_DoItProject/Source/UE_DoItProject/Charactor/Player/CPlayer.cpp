#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Component/Player/CPlayerAttackComp.h"
#include "Component/Player/CPlayerMontageComp.h"
#include "Component/Player/CPlayerEquipComp.h"

#include "Item/CPL_Sword.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default Setting
	{
		GetCharacterMovement()->MaxWalkSpeed = 400.0f;

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	}

	// Camera
	{
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
		SpringArmComp->SetupAttachment(GetMesh());

		CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
		CameraComp->SetupAttachment(SpringArmComp);
	}

	// Setting
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
		AttackComponent = CreateDefaultSubobject<UCPlayerAttackComp>("AttackComp");
		MontageComponent = CreateDefaultSubobject<UCPlayerMontageComp>("MontageComp");
		EquipComponent = CreateDefaultSubobject<UCPlayerEquipComp>("EquipmentComponent");
	}

	// State Cast Montage
	// @딱히 둘데가 없어서 여기뒀다..
	FString strPath;
	{
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/Mage_Cast_Montage.Mage_Cast_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> cast(*strPath);
		if (cast.Succeeded())
			MageStateCastMontage = cast.Object;
	}

	// Mage State
	{
		// Left Hand Particle
		LeftParticle = CreateDefaultSubobject<UParticleSystemComponent>("LeftParticle");
		LeftParticle->SetupAttachment(GetMesh());
		LeftParticle->bAutoActivate = true;
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/P_LightingHand.P_LightingHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> leftParticle(*strPath);
		if (leftParticle.Succeeded())
			LeftParticle->SetTemplate(leftParticle.Object);
		LeftParticle->SetRelativeLocation(FVector(0.0f));

		// Right Hand Particle
		RightParticle = CreateDefaultSubobject<UParticleSystemComponent>("RightParticle");
		RightParticle->SetupAttachment(GetMesh());
		RightParticle->bAutoActivate = true;
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/P_LightingHand.P_LightingHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> rightParticle(*strPath);
		if (rightParticle.Succeeded())
			RightParticle->SetTemplate(rightParticle.Object);
		RightParticle->SetRelativeLocation(FVector(0.0f));
	}
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	EquipComponent->GetDisplayItem(0)->GetStaticMeshComp()->SetVisibility(false);

	// Scaling
	LeftParticle->SetWorldScale3D(FVector(3.f));
	RightParticle->SetWorldScale3D(FVector(3.f));
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// CLog::Print(static_cast<int>(CurrentStateType));
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// In APlayer BindAction
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
	IfFalseRet(bCanMove);

	// ACharacter
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

void ACPlayer::OnEvade()
{
	// @IF TRUE
	IfTrueRet(bEvade);
	IfTrueRet(GetCharacterMovement()->IsFalling());
	IfTrueRet(EquipComponent->GetEquiping()); //@Equping Check

	// @IF FALSE
	IfFalseRet(bCanMove);

	bEvade = true;

	// Montage Sort & PlayNum
	UINT Sort = static_cast<UINT>(MontageSort::Move);
	// @회피동작의 기본 Anim = 0 - (앞쪽 회피)
	UINT PlayAnimNum = 0;
	// @회피동작 방향
	EvadeDirection = GetActorForwardVector();

	// 회피 4 방향 동작하게하기 ( 추후 Test )
	// 지금은 앞구르기만으로 바꿈
	#pragma region Test Evade 4Way 
	//// @Forward
	//if (GetInputAxisValue(L"MoveForward") > 0.0f)
	//{
	//	PlayAnimNum = 0;
	//	EvadeDirection = GetActorForwardVector();
	//	// CLog::Print(L"Forward In!!");
	//}
	//// @Back
	//else if (GetInputAxisValue(L"MoveForward") < 0.0f)
	//{
	//	PlayAnimNum = 1;
	//	EvadeDirection = (-1) * GetActorForwardVector();
	//	// CLog::Print(L"Back In!!");
	//}
	//// @Left
	//else if (GetInputAxisValue(L"MoveRight") < 0.0f)
	//{
	//	PlayAnimNum = 2;
	//	EvadeDirection = (-1) * GetActorRightVector();
	//}
	//// @Right
	//else if (GetInputAxisValue(L"MoveRight") > 0.0f)
	//{
	//	PlayAnimNum = 3;
	//	EvadeDirection = GetActorRightVector();
	//}
	#pragma endregion	


	// Montage 실행
	MontageComponent->PlayAnimation
	(
		0,
		PlayAnimNum,
		Sort,
		1.0f,
		false
	);
}

// @Player 공격 상태를 바꾸는 함수
void ACPlayer::OnSwapState()
{
	// @IF TRUE
	IfTrueRet(bEvade); //@Evade Check
	IfTrueRet(GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(EquipComponent->GetEquiping()); //@Equping Check

	// @Equping Set 'False' From Notify
	EquipComponent->SetEquiping(true);

	// State Swap
	int Type = static_cast<int>(CurrentStateType);
	++Type;
	(Type == static_cast<int>(StateType::END))
		? CurrentStateType = static_cast<StateType>(0)
		: CurrentStateType = static_cast<StateType>(Type);

	ACPL_Sword* Sword = Cast<ACPL_Sword>(EquipComponent->GetDisplayItem(0));
	check(Sword);

	// @Mage State
	if (CurrentStateType == StateType::MAGE)
	{
		ActorAnimMonPlay(MageStateCastMontage, 1.5f, false);
		// Sword->UnEquip(); - UnEquip 은 필요없음. Attach, DeathPosition 이 같음.
		Sword->GetStaticMeshComp()->SetVisibility(false);

		LeftParticle->SetVisibility(true); // MageHandEffect
		RightParticle->SetVisibility(true); // MageHandEffect
	}
	// @Sword State
	else if (CurrentStateType == StateType::SWORD)
	{
		Sword->OnEquip();
		Sword->GetStaticMeshComp()->SetVisibility(true);

		LeftParticle->SetVisibility(false); // MageHandEffect
		RightParticle->SetVisibility(false); // MageHandEffect
	}
}

void ACPlayer::OffEvade()
{
	bEvade = false;
}

void ACPlayer::OnHit(AActor * AttackActor, UINT HitAnimNum, float AnimSpeed)
{
	IfTrueRet(bDeath);

	// Delegate 실행 - 상태값 초기화.
	OnHitResetState.Broadcast(this);

	FVector Location = GetActorLocation();
	FVector OtherLocation = AttackActor->GetActorLocation();

	// Hit Animation 실행
	ActorAnimMonPlay(HitMontages[HitAnimNum], AnimSpeed, true);

	// 우선 공격한 방향으로 돌리기.
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
		// 만약 캐릭터가 몽타주 실행 중이라면,
		// RetTrue(bMontagePlaying);
		IfTrueRet(GetMesh()->GetAnimInstance()->Montage_IsPlaying(CurrentMontage));
	}

	PlayAnimMontage(Montage, Speed);
	CurrentMontage = Montage;
}
