#include "CPL_BlendCameraComp.h"
#include "Global.h"

#include "Charactor/Player/CPlayer.h"
#include "Camera/CPL_BlendCameraActor.h"

UCPL_BlendCameraComp::UCPL_BlendCameraComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Setting Position & Rotation
	{
		//@RightFace Blend Camera
		Pos_RightFaceBlendCamera = FVector(20.0f, 470.0f, 250.0f);
		Rot_RightFaceBlendCamera = FRotator(-10.0f, -90.0f, 0.0f);

		//@LeftFace Blend Camera
		Pos_LeftFaceBlendCamera = FVector(20.0f, -470.0f, 250.0f);
		Rot_LeftFaceBlendCamera = FRotator(-10.0f, 90.0f, 0.0f);

		//@ForwardFace Blend Camera
		Pos_ForwardFaceBlendCamera = FVector(660.0f, -350.0f, 480.0f);
		Rot_ForwardFaceBlendCamera = FRotator(-30.0f, 160.0f, 0.0f);

		//@BottomFace Blend Camera
		Pos_BottomFaceBlendCamera = FVector(300.0f, 800.0f, -50.0f);
		Rot_BottomFaceBlendCamera = FRotator(20.0f, -110.0f, 0.0f);

		//@BackFace Blend Camera
		Pos_BackFaceBlendCamera = FVector(0.0f, 0.0f, -0.0f);
		Rot_BackFaceBlendCamera = FRotator(0.0f, -0.0f, 0.0f);
	}
}


void UCPL_BlendCameraComp::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	// Create BlendCameraActor
	{
		#pragma region RightSide Face
		//@RightSide Face - 오른쪽 측면
		ACPL_BlendCameraActor* RightSideFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		RightSideFaceCamera->SetActorLocation(Pos_RightFaceBlendCamera);
		RightSideFaceCamera->SetActorRotation(Rot_RightFaceBlendCamera);

		//@Set Owner
		RightSideFaceCamera->SetOwner(GetOwner());
		RightSideFaceCamera->AttachToComponent
		(
			Player->GetRootComponent()
			, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true)
			, NAME_None
		);
		#pragma endregion

		#pragma region LeftSide Face
		//@LeftSide Face - 왼쪽 측면
		ACPL_BlendCameraActor* LeftSideFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		LeftSideFaceCamera->SetActorLocation(Pos_LeftFaceBlendCamera);
		LeftSideFaceCamera->SetActorRotation(Rot_LeftFaceBlendCamera);

		//@Set Owner
		LeftSideFaceCamera->SetOwner(GetOwner());
		LeftSideFaceCamera->AttachToComponent
		(
			Player->GetRootComponent()
			, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true)
			, NAME_None
		);
		#pragma endregion

		#pragma region Forward Face
		//@Forward Face - 전방
		ACPL_BlendCameraActor* ForwardFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		ForwardFaceCamera->SetActorLocation(Pos_ForwardFaceBlendCamera);
		ForwardFaceCamera->SetActorRotation(Rot_ForwardFaceBlendCamera);

		//@Set Owner
		ForwardFaceCamera->SetOwner(GetOwner());
		ForwardFaceCamera->AttachToComponent
		(
			Player->GetRootComponent()
			, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true)
			, NAME_None
		);
		#pragma endregion
				#pragma region Bottom Face
		//@Bottom Face - 아래
		ACPL_BlendCameraActor* BottomFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		BottomFaceCamera->SetActorLocation(Pos_BottomFaceBlendCamera);
		BottomFaceCamera->SetActorRotation(Rot_BottomFaceBlendCamera);

		//@Set Owner
		BottomFaceCamera->SetOwner(GetOwner());
		BottomFaceCamera->AttachToComponent
		(
			Player->GetRootComponent()
			, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true)
			, NAME_None
		);
		#pragma endregion

		// @Insert
		BlendCameras.Emplace(RightSideFaceCamera);
		BlendCameras.Emplace(LeftSideFaceCamera);
		BlendCameras.Emplace(ForwardFaceCamera);
		BlendCameras.Emplace(BottomFaceCamera);
	}	
}

void UCPL_BlendCameraComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

AActor * UCPL_BlendCameraComp::GetBlendCamera(EBlendCameraPositionType Type)
{
	uint8 TypeNum = static_cast<uint8>(Type);
	if (TypeNum > static_cast<uint8>(BlendCameras.Num()) - 1)
	{
		return nullptr;
	}

	return Cast<AActor>(BlendCameras[TypeNum]);
}

void UCPL_BlendCameraComp::AllResetBlendCameraDefaultValue()
{
	//@Right
	BlendCameras[0]->SetActorLocation(Pos_RightFaceBlendCamera);
	BlendCameras[0]->SetActorRotation(Rot_RightFaceBlendCamera);

	//@Left
	BlendCameras[1]->SetActorLocation(Pos_LeftFaceBlendCamera);
	BlendCameras[1]->SetActorRotation(Rot_LeftFaceBlendCamera);

	//@Forward
	BlendCameras[2]->SetActorLocation(Pos_ForwardFaceBlendCamera);
	BlendCameras[2]->SetActorRotation(Rot_ForwardFaceBlendCamera);

	//@Bottom
	BlendCameras[3]->SetActorLocation(Pos_BottomFaceBlendCamera);
	BlendCameras[3]->SetActorRotation(Rot_BottomFaceBlendCamera);

	//@Back
	BlendCameras[4]->SetActorLocation(Pos_BackFaceBlendCamera);
	BlendCameras[4]->SetActorRotation(Rot_BackFaceBlendCamera);
}

