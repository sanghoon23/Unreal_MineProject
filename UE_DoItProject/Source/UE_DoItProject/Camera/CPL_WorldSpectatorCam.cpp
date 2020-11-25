#include "CPL_WorldSpectatorCam.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WidgetComponent.h"

ACPL_WorldSpectatorCam::ACPL_WorldSpectatorCam()
{
	PrimaryActorTick.bCanEverTick = true;

	//@CapsuleComponent
	GetCapsuleComponent()->SetCollisionProfileName("Spectator");

	//@WorldCam Charactor 에 맞게 Movement 수정
	GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	GetCharacterMovement()->NavAgentProps.bCanFly = true;
	GetCharacterMovement()->Mass = 30000.0f;
	GetCharacterMovement()->BrakingDecelerationFlying = 10000.0f;
	GetCharacterMovement()->MaxFlySpeed = 1500.0f;
	GetCharacterMovement()->MaxAcceleration = 4096.0f;

	// Camera
	{
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
		SpringArmComp->SetupAttachment(GetMesh());

		CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera");
		CameraComp->SetupAttachment(SpringArmComp);
	}

	// Setting CharacterMovement
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

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
}

void ACPL_WorldSpectatorCam::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPL_WorldSpectatorCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPL_WorldSpectatorCam::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//@Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPL_WorldSpectatorCam::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPL_WorldSpectatorCam::OnMoveRight);
	PlayerInputComponent->BindAxis("MoveUpper", this, &ACPL_WorldSpectatorCam::OnMoveUpper);
	PlayerInputComponent->BindAxis("Turn", this, &ACPL_WorldSpectatorCam::OnTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &ACPL_WorldSpectatorCam::OnLookUp);
	PlayerInputComponent->BindAxis("Zoom", this, &ACPL_WorldSpectatorCam::OnZoom);

	//@Action
	PlayerInputComponent->BindAction("TestKeyAction", EInputEvent::IE_Pressed, this, &ACPL_WorldSpectatorCam::BindRespawnPlayerFunc);
}

void ACPL_WorldSpectatorCam::SetRespawnPlayer(ACPlayer * RespawnPlayerPtr)
{
	check(RespawnPlayerPtr);
	RespawnPlayer = RespawnPlayerPtr;
}

void ACPL_WorldSpectatorCam::BindRespawnPlayerFunc()
{
	UWorld* const World = GetWorld();

	//@Controller Disable Input
	APlayerController* const PC = Cast<APlayerController>(GetController());
	check(PC);
	DisableInput(PC);

	//@SpawnActor 하면 안됨. Bp 로 설정되서 들어가지 않음,
	////@Create Player
	//FTransform Transform = FTransform::Identity;
	//Transform.SetLocation(GetActorLocation());
	//Transform.SetRotation(FQuat(CameraComp->GetComponentRotation()));
	//FActorSpawnParameters Params;
	//ACPlayer* RetPlayer = World->SpawnActor<ACPlayer>
	//(
	//	ACPlayer::StaticClass(), Transform, Params
	//);
	//check(RetPlayer);

	//@기존의 전역 PlayerData 삭제.
	//TODO : 캐릭터 BP 로 Spawn 해야함...........SpawnActor 는 아니잖아..아는거잖아..

	//@WorldSpectatorCam Actor 에 들어와서 초기화 된 변수들 다시 리셋
	{
		GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Walking;
		GetCharacterMovement()->NavAgentProps.bCanFly = false;
		GetCharacterMovement()->Mass = 100.0f;
		GetCharacterMovement()->BrakingDecelerationFlying = 0.0f;
		GetCharacterMovement()->MaxFlySpeed = 600.0f;
		GetCharacterMovement()->MaxAcceleration = 2048.0f;
	}

	//@Player 빙의, 관중카메라 모드 해제
	UnPossessed();

	//@빙의하기 전 세팅 초기화.
	check(RespawnPlayer);

	//@해당 Controller 빙의.
	PC->Possess(RespawnPlayer);
	RespawnPlayer->SetActorLocation(GetActorLocation());
	RespawnPlayer->OnInit();

	//@Death
	Destroy();
}

void ACPL_WorldSpectatorCam::OnMoveForward(float Value)
{
	FRotator Rotation = GetControlRotation();
	FRotator Temp = FRotator(0, Rotation.Yaw, 0);
	const FVector Forward = FQuat(Temp).GetForwardVector();

	//FVector Location = GetActorLocation();
	//Location += Forward * Value * SpectatorCameraSpeed;
	//SetActorLocation(Location);

	/* Value * MultipleInputSpeed */
	AddMovementInput(Forward, Value * SpectatorCameraSpeed);
}

void ACPL_WorldSpectatorCam::OnMoveRight(float Value)
{
	FRotator Rotation = GetControlRotation();
	FRotator Temp = FRotator(0, Rotation.Yaw, 0);
	const FVector Right = FQuat(Temp).GetRightVector();

	//FVector Location = GetActorLocation();
	//Location += Right * Value * SpectatorCameraSpeed;
	//SetActorLocation(Location);

	/* Value * MultipleInputSpeed */
	AddMovementInput(Right, Value * SpectatorCameraSpeed);
}

void ACPL_WorldSpectatorCam::OnMoveUpper(float Value)
{
	const FVector UpVec = FVector(0.0f, 0.0f, 1.0f);
	//FVector Location = GetActorLocation();
	//Location += UpVec * Value * SpectatorCameraSpeed;
	//SetActorLocation(Location);

	/* Value * MultipleInputSpeed */
	AddMovementInput(UpVec, Value * SpectatorCameraSpeed);
}

void ACPL_WorldSpectatorCam::OnTurn(float Value)
{
	AddControllerYawInput(Value);
}

void ACPL_WorldSpectatorCam::OnLookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ACPL_WorldSpectatorCam::OnZoom(float Value)
{
	float Length = SpringArmComp->TargetArmLength += (ZoomSpeed * Value);
	Length = FMath::Clamp(Length, MinWheel, MaxWheel);
	SpringArmComp->TargetArmLength = Length;
	//CLog::Print(SpringArmComp->TargetArmLength);
}
