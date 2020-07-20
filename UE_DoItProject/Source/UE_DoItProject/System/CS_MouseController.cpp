#include "CS_MouseController.h"
#include "Global.h"

#include "Interface/IC_Monster.h"
#include "Actor/Decal/CDecalActor_WithMouse.h"
#include "Charactor/Player/CPlayer.h"
#include "System/CS_TargetingSystem.h"

//UI
#include "UI/HUD_Main.h"
#include "UI/Widget/WG_TargetInfo.h"

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

	//@Player
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	//@Spawn DecalActor
	FTransform Transform = FTransform::Identity;
	DecalActor = GetWorld()->SpawnActor<ACDecalActor_WithMouse>(ACDecalActor_WithMouse::StaticClass(), Transform);
	//@Warning - 회전 해주어야 Material 이 나옴
	DecalActor->SetDecalCompRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DecalActor->SetDecalCompMat(DecalMatCanUsingRange);
	DecalActor->SetDecalSize(DecalActorCircleSize);
	DecalActor->GetRootComponent()->SetVisibility(false);

	//@UI
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC != nullptr)
	{
		AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		check(MainHUD);
		TargetInfoWidget = MainHUD->GetWidgetTargetInfo();
	}
}

void UCS_MouseController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@Using Decal
	if (bUsingDecalMouseControl == true)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), ControllerIndex);
		if (PC != nullptr)
		{
			FVector HitLocation;
			bool bRay = UsingDecalMouseRayAndHit(PC, HitLocation);
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
					OffUsingDecalMouseControl();
				}

			}//(bRay == true)

			//@오른쪽 Mouse 버튼 누르면 해제
			if (PC->IsInputKeyDown(EKeys::RightMouseButton))
			{
				MouseState = EMouseState::NONE;
				OffUsingDecalMouseControl();
			}

		}//(PC != nullptr)
	}
	//@몬스터 선택
	else if (bUsingDecalMouseControl == false)
	{
		DecalActor->GetRootComponent()->SetVisibility(false);

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), ControllerIndex);
		if (PC != nullptr)
		{
			if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
			{
				TargetMouseRayAndHit(PC);
			}
		}
	}
}

void UCS_MouseController::OnUsingDecalMouseControl(FVector DecalCircleSize, AActor* StandardTarget, float StandardRange)
{
	MouseState = EMouseState::WAIT;
	bUsingDecalMouseControl = true;

	//@Set Decal Size
	DecalActorCircleSize = DecalCircleSize;
	DecalActor->SetDecalSize(DecalActorCircleSize);

	//@Set Standard
	if (StandardTarget != nullptr)
	{
		TargetMouseStandard = StandardTarget;
		MouseRangeWithTarget = StandardRange;
	}
	else
	{
		TargetMouseStandard = nullptr;
		MouseRangeWithTarget = 0.0f;
	}
}

void UCS_MouseController::OffUsingDecalMouseControl()
{
	bUsingDecalMouseControl = false;

	DecalActorCircleSize = FVector(0.0f);

	TargetMouseStandard = nullptr;
	MouseRangeWithTarget = 0.0f;
}

bool UCS_MouseController::TargetMouseRayAndHit(APlayerController * PC)
{
	check(PC);

	//@UnProjection
	FVector WorldLocation;
	FVector WorldDirection;
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	//@Calc Hit
	FVector RayStart = WorldLocation;
	FVector RayEnd = WorldLocation + (WorldDirection * 10000.0f);
	TArray<AActor *> Ignore;
	EDrawDebugTrace::Type Debug
		= (bDebug == true) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), RayStart, RayEnd, 
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false, Ignore, Debug, HitResult, true
	);

	if (bHit == true)
	{
		UCS_TargetingSystem* TargetSystem = Player->GetTargetingSystem();
		check(TargetSystem);

		TargetSystem->OnFindTargets(HitResult.GetActor()->GetActorLocation(), 10.0f);
	}
	else return false;

	return true;
}

bool UCS_MouseController::UsingDecalMouseRayAndHit(APlayerController* PC, FVector& HitedLocation)
{
	check(PC);

	//@UnProjection
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
		GetWorld(), RayStart, RayEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
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

			//@Mouse 거리가 있을 때,
			if (TargetMouseStandard != nullptr)
			{
				FVector CheckRangeVec = TargetMouseStandard->GetActorLocation() - RetLocation;
				if (CheckRangeVec.Size() > MouseRangeWithTarget)
				{
					float temp = CheckRangeVec.Size() - MouseRangeWithTarget;
					CheckRangeVec.Normalize();
					CheckRangeVec *= (-1);
					RetLocation =
						TargetMouseStandard->GetActorLocation() + (CheckRangeVec * MouseRangeWithTarget);
				}
			}

			//@Set DecalActor
			DecalActor->SetActorLocation(RetLocation);

			//@HitedLocation
			HitedLocation = RetLocation;

			return true;
		}
	}

	return false;
}

