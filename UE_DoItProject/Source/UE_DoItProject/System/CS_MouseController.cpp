#include "CS_MouseController.h"
#include "Global.h"

#include "Actor/Decal/CDecalActor_WithMouse.h"

UCS_MouseController::UCS_MouseController()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString strPath = L"";

	#pragma region Matrial Load
	//Material Load
	strPath = L"Material'/Game/_Mine/UseMaterial/Decal/Mat_DecalGreen.Mat_DecalGreen'";
	ConstructorHelpers::FObjectFinder<UMaterialInterface> CanUseMat(*strPath);
	if (CanUseMat.Succeeded())
	{
		DecalMatCanUsingRange = CanUseMat.Object;
	}

	strPath = L"Material'/Game/_Mine/UseMaterial/Decal/Mat_DecalRed.Mat_DecalRed'";
	ConstructorHelpers::FObjectFinder<UMaterialInterface> CanNotUseMat(*strPath);
	if (CanNotUseMat.Succeeded())
	{
		DecalMatCanNotUsingRange = CanNotUseMat.Object;
	}


	#pragma endregion

}

void UCS_MouseController::BeginPlay()
{
	Super::BeginPlay();

	bDebug = true;

	//@Spawn DecalActor
	FTransform Transform = FTransform::Identity;
	DecalActor = GetWorld()->SpawnActor<ACDecalActor_WithMouse>(ACDecalActor_WithMouse::StaticClass(), Transform);
	DecalActor->SetDecalCompRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DecalActor->SetDecalCompMat(DecalMatCanUsingRange);
	DecalActor->SetDecalSize(FVector(300.0f));
	DecalActor->GetRootComponent()->SetVisibility(false);
}

void UCS_MouseController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bUsingControl == true)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), ControllerIndex);
		if (PC != nullptr)
		{
			FVector HitLocation;
			bool bRay = MouseRayAndHit(PC, HitLocation);
			if (bRay == true)
			{
				//@Set Visibility
				DecalActor->GetRootComponent()->SetVisibility(true);

				//@Set Material
				DecalActor->SetDecalCompMat(DecalMatCanUsingRange);

				if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
				{
					//@좌표 넘겨주기.
					ClickPoint = HitLocation;
					MouseState = EMouseState::CHECKINGPOINT;
					OffMouseControl();
				}

			}//(bRay == true)

			//@오른쪽 Mouse 버튼 누르면 해제
			if (PC->IsInputKeyDown(EKeys::RightMouseButton))
			{
				MouseState = EMouseState::NONE;
				OffMouseControl();
			}

		}//(PC != nullptr)
	}
	else if (bUsingControl == false)
	{
		DecalActor->GetRootComponent()->SetVisibility(false);
	}
}

void UCS_MouseController::OnMouseControl()
{
	MouseState = EMouseState::WAIT;
	bUsingControl = true;
}

void UCS_MouseController::OffMouseControl()
{
	bUsingControl = false;
}

bool UCS_MouseController::MouseRayAndHit(APlayerController* PC, FVector& HitedLocation)
{
	check(PC);

	//@DeProjection
	FVector WorldLocation;
	FVector WorldDirection;
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	//@Calc Hit
	FVector RayStart = WorldLocation;
	FVector RayEnd = WorldLocation + (WorldDirection * 10000.0f);
	TArray<AActor *> Ignore;
	EDrawDebugTrace::Type Debug 
		= (bDebug == true) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::LineTraceMulti
	(
		GetWorld(), RayStart, RayEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, Ignore, Debug, HitResults, true
	);

	//@Ret Value
	FVector RetLocation = FVector(0.0f);
	for (FHitResult HitResult : HitResults)
	{
		if (Cast<APawn>(HitResult.GetActor()) != nullptr)
		{
			//@Set Other DecalMaterial
			DecalActor->SetDecalCompMat(DecalMatCanNotUsingRange);
			continue;
		}

		//#Temp 임시
		//@바닥이랑 닿을 때,
		if (HitResult.GetActor()->GetName().Contains("Floor"))
		{
			RetLocation = HitResult.Location;
			RetLocation.Z += 0.1;
			DecalActor->SetActorLocation(RetLocation);

			//@HitedLocation
			HitedLocation = RetLocation;

			return true;
		}
	}

	return false;
}

