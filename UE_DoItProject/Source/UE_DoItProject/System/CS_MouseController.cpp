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

	//@Player
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	//@Spawn DecalActor
	FTransform Transform = FTransform::Identity;
	DecalActor = GetWorld()->SpawnActor<ACDecalActor_WithMouse>(ACDecalActor_WithMouse::StaticClass(), Transform);
	//@Warning - ȸ�� ���־�� Material �� ����
	DecalActor->SetDecalCompRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DecalActor->SetDecalCompMat(DecalMatCanUsingRange);
	DecalActor->SetDecalSize(DecalActorCircleSize);
	DecalActor->GetRootComponent()->SetVisibility(false);

	//@UI
	//APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APlayerController* PC = Cast<APlayerController>(Player->GetController());
	if (PC != nullptr)
	{
		//AHUD_Main* MainHUD = Cast<AHUD_Main>(PC->GetHUD());
		AHUD_Main* MainHUD = PC->GetHUD<AHUD_Main>();
		//check(MainHUD);
		if (MainHUD != nullptr)
		{
			TargetInfoWidget = MainHUD->GetWidgetTargetInfo();
		}
	}
}

void UCS_MouseController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	check(Character);
	APlayerController* PC = Cast<APlayerController>(Character->GetController());

	//@Using Decal - ĳ������ ��ų�� ������ ��,
	if (bUsingDecalMouseControl == true && PC != nullptr)
	{
		FVector HitLocation;
		bool bRay = UsingDecalMouseRayAndHit(PC, HitLocation, MouseCollisionChannel);
		if (bRay == true)
		{
			//@Set Visibility
			DecalActor->GetRootComponent()->SetVisibility(true);

			//@Set Material
			DecalActor->SetDecalCompMat(DecalMatCanUsingRange);

			if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
			{
				//@��ǥ �Ѱ��ֱ�.
				ClickPoint = HitLocation;
				MouseState = EMouseState::CHECKINGPOINT;
				OffUsingDecalMouseControl();
			}

		}//(bRay == true)

		//@������ Mouse ��ư ������ ����
		if (PC->IsInputKeyDown(EKeys::RightMouseButton))
		{
			MouseState = EMouseState::NONE;
			OffUsingDecalMouseControl();
		}
	}
	//@���콺�� ���� ������ �� �� �ִ� ����
	else if (bUsingDecalMouseControl == false && PC != nullptr)
	{
		DecalActor->GetRootComponent()->SetVisibility(false);
		if (PC->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			TargetMouseRayAndHit(PC, ECollisionChannel::ECC_Visibility);
		}
	}
}

void UCS_MouseController::OnUsingDecalMouseControl(FVector DecalCircleSize, AActor* StandardTarget, float StandardRange, ECollisionChannel Channel)
{
	bUsingDecalMouseControl = true;

	MouseCollisionChannel = Channel;
	MouseState = EMouseState::WAIT;

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
		UE_LOG(LogTemp, Warning, L"CS_MouseController OnUsingDecalMouseControlFunc Target NULL!!");
	}
}

void UCS_MouseController::OffUsingDecalMouseControl()
{
	bUsingDecalMouseControl = false;

	MouseCollisionChannel = ECollisionChannel::ECC_Visibility;
	/* EMouseState �� OffUsingDecal ���� CheckPoint ������ ���� ������������. */

	DecalActorCircleSize = FVector(0.0f);

	TargetMouseStandard = nullptr;
	MouseRangeWithTarget = 0.0f;
}

bool UCS_MouseController::TargetMouseRayAndHit(APlayerController * PC, ECollisionChannel LineTraceChannel)
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
		UEngineTypes::ConvertToTraceType(LineTraceChannel),
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

bool UCS_MouseController::UsingDecalMouseRayAndHit(APlayerController* PC, FVector& HitedLocation, ECollisionChannel LineTraceChannel)
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
		UEngineTypes::ConvertToTraceType(LineTraceChannel),
		false, Ignore, Debug, HitResults, true
	);

	//@Ret Value
	FVector RetLocation = FVector(0.0f);
	for (FHitResult HitResult : HitResults)
	{
		//if (Cast<APawn>(HitResult.GetActor()) != nullptr) //@HitResultActor �� Pawn �̶��
		//{
		//	//@Set Other DecalMaterial
		//	DecalActor->SetDecalCompMat(DecalMatCanNotUsingRange);
		//	continue;
		//}

		CLog::Print(HitResult.GetActor()->GetName());

		//#1105_Temp �ӽ�
		//@�ٴ��̶� ���� ��,
		if (HitResult.GetActor()->GetName().Contains(HitFloorContainName))
		{
			RetLocation = HitResult.Location;
			RetLocation.Z += 1.0f;
			CLog::Print(HitResult.Location);
			//@ĳ������ ��� ���̸� ������.
			//ACharacter* CHR_TagetMouseStandard = Cast<ACharacter>(TargetMouseStandard);
			//if (CHR_TagetMouseStandard != nullptr)
			//	RetLocation.Z += CHR_TagetMouseStandard->GetDefaultHalfHeight();
			//else
			//	RetLocation.Z += 0.1;

			//CLog::Print(RetLocation);

			//@Mouse �Ÿ��� ���� ��, (������ �Ѿ�� �ʰ� �ϱ� ����)
			if (TargetMouseStandard != nullptr)
			{
				FVector CheckRangeVec = TargetMouseStandard->GetActorLocation() - RetLocation;
				if (CheckRangeVec.Size() > MouseRangeWithTarget)
				{
					CheckRangeVec.Normalize();
					CheckRangeVec *= (-1);
					RetLocation =
						TargetMouseStandard->GetActorLocation() + (CheckRangeVec * MouseRangeWithTarget);
				}
			}

			HitedLocation = RetLocation;

			//RetLocation.Z += 300.0f;
			DecalActor->SetActorLocation(RetLocation);

			return true;
		}
		else continue;
	}//for(HitResult)

	return false;
}

