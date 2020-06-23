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
		Pos_ForwardFaceBlendCamera = FVector(660.0f, 0.0f, 450.0f);
		Rot_ForwardFaceBlendCamera = FRotator(-30.0f, 180.0f, 0.0f);

		//@BottomFace Blend Camera
		Pos_BottomFaceBlendCamera = FVector(660.0f, 0.0f, -300.0f);
		Rot_BottomFaceBlendCamera = FRotator(25.0f, 180.0f, 0.0f);

		//@BackFace Blend Camera
		Pos_BackFaceBlendCamera = FVector(-660.0f, 0.0f, 450.0f);
		Rot_BackFaceBlendCamera = FRotator(-25.0f, 0.0f, 0.0f);
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
		//@1. RightSide Face - 오른쪽 측면
		ACPL_BlendCameraActor* RightSideFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		RightSideFaceCamera->GetRootComponent()->SetRelativeLocation(Pos_RightFaceBlendCamera);
		RightSideFaceCamera->GetRootComponent()->SetRelativeRotation(Rot_RightFaceBlendCamera);

		//@Set Owner
		RightSideFaceCamera->SetOwner(GetOwner());
		RightSideFaceCamera->AttachToComponent
		(
			Player->GetMesh()
			, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
			, "root"
		);
		#pragma endregion

		#pragma region LeftSide Face
		//@2. LeftSide Face - 왼쪽 측면
		ACPL_BlendCameraActor* LeftSideFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		LeftSideFaceCamera->GetRootComponent()->SetRelativeLocation(Pos_LeftFaceBlendCamera);
		LeftSideFaceCamera->GetRootComponent()->SetRelativeRotation(Rot_LeftFaceBlendCamera);

		//@Set Owner
		LeftSideFaceCamera->SetOwner(GetOwner());
		LeftSideFaceCamera->AttachToComponent
		(
			Player->GetMesh()
			, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
			, "root"
		);
		#pragma endregion

		#pragma region Forward Face
		//@3. Forward Face - 전방
		ACPL_BlendCameraActor* ForwardFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		ForwardFaceCamera->GetRootComponent()->SetRelativeLocation(Pos_ForwardFaceBlendCamera);
		ForwardFaceCamera->GetRootComponent()->SetRelativeRotation(Rot_ForwardFaceBlendCamera);

		//@Set Owner
		ForwardFaceCamera->SetOwner(GetOwner());
		ForwardFaceCamera->AttachToComponent
		(
			Player->GetMesh()
			, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
			, "root"
		);
		#pragma endregion

		#pragma region Bottom Face
		//@4. Bottom Face - 아래
		ACPL_BlendCameraActor* BottomFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		BottomFaceCamera->GetRootComponent()->SetRelativeLocation(Pos_BottomFaceBlendCamera);
		BottomFaceCamera->GetRootComponent()->SetRelativeRotation(Rot_BottomFaceBlendCamera);

		//@Set Owner
		BottomFaceCamera->SetOwner(GetOwner());
		BottomFaceCamera->AttachToComponent
		(
			Player->GetMesh()
			, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
			, "root"
		);
		#pragma endregion

		#pragma region Back Face
		//@5. Back Face - 후방
		ACPL_BlendCameraActor* BackFaceCamera = GetWorld()->SpawnActor<ACPL_BlendCameraActor>(ACPL_BlendCameraActor::StaticClass());
		BackFaceCamera->GetRootComponent()->SetRelativeLocation(Pos_BackFaceBlendCamera);
		BackFaceCamera->GetRootComponent()->SetRelativeRotation(Rot_BackFaceBlendCamera);

		//@Set Owner
		BackFaceCamera->SetOwner(GetOwner());
		BackFaceCamera->AttachToComponent
		(
			Player->GetMesh()
			, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true)
			, "root"
		);
		#pragma endregion

		// @Insert
		BlendCameras.Emplace(RightSideFaceCamera);
		BlendCameras.Emplace(LeftSideFaceCamera);
		BlendCameras.Emplace(ForwardFaceCamera);
		BlendCameras.Emplace(BottomFaceCamera);
		BlendCameras.Emplace(BackFaceCamera);
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
	BlendCameras[0]->GetRootComponent()->SetRelativeLocation(Pos_RightFaceBlendCamera);
	BlendCameras[0]->GetRootComponent()->SetRelativeRotation(Rot_RightFaceBlendCamera);

	//@Left
	BlendCameras[1]->GetRootComponent()->SetRelativeLocation(Pos_LeftFaceBlendCamera);
	BlendCameras[1]->GetRootComponent()->SetRelativeRotation(Rot_LeftFaceBlendCamera);

	//@Forward
	BlendCameras[2]->GetRootComponent()->SetRelativeLocation(Pos_ForwardFaceBlendCamera);
	BlendCameras[2]->GetRootComponent()->SetRelativeRotation(Rot_ForwardFaceBlendCamera);

	//@Bottom
	BlendCameras[3]->GetRootComponent()->SetRelativeLocation(Pos_BottomFaceBlendCamera);
	BlendCameras[3]->GetRootComponent()->SetRelativeRotation(Rot_BottomFaceBlendCamera);

	//@Back
	BlendCameras[4]->GetRootComponent()->SetRelativeLocation(Pos_BackFaceBlendCamera);
	BlendCameras[4]->GetRootComponent()->SetRelativeRotation(Rot_BackFaceBlendCamera);
}

