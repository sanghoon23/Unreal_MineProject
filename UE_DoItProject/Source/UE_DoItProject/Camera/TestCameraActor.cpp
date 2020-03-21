#include "TestCameraActor.h"
#include "Global.h"
#include "Curves/CurveFloat.h"
#include "Components/SplineComponent.h"

#include "Charactor/Player/CPlayer.h"

ATestCameraActor::ATestCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Spline
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(RootComponent);

	// Creat Curve
	FString path = L"CurveFloat'/Game/_Mine/_MyBlueprint/Curve/CameraFloatTest.CameraFloatTest'";
	ConstructorHelpers::FObjectFinder<UCurveFloat> curve(*path);
	if (curve.Succeeded())
		CurveFloat = curve.Object;
}

void ATestCameraActor::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(Player);

	FSplinePoint Point1;
	Point1.Position = FVector(-90.0f, 500.0f, 100.0f);
	Point1.Rotation = FRotator(0.0f, 0.0f, -90.0f);
	Spline->AddPoint(Point1);

	FSplinePoint Point2;
	Point2.Position = FVector(-90.0f, 1000.0f, 100.0f);
	Point2.Rotation = FRotator(0.0f, 0.0f, -90.0f);
	Spline->AddPoint(Point2);
}

void ATestCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Timeline.IsPlaying())
		Timeline.TickTimeline(DeltaTime);
}

void ATestCameraActor::OnPlay()
{
	UpdateCameraMove(CurveFloat);
}

void ATestCameraActor::UpdateCameraMove(UCurveFloat * CurveFloat)
{
	Timeline = FTimeline();

	FOnTimelineFloat progress;
	progress.BindUFunction(this, "UpdateActorLocationWithSpline");

	Timeline.AddInterpFloat(CurveFloat, progress);
	Timeline.SetLooping(false);
	Timeline.SetTimelineLengthMode(TL_LastKeyFrame);
	Timeline.PlayFromStart();
}

void ATestCameraActor::UpdateActorLocationWithSpline(float fValue)
{
	FVector PlayerLocation = Player->GetActorLocation();

	float length = Spline->GetSplineLength();
	FVector SplineLocation
		= Spline->GetLocationAtDistanceAlongSpline(fValue * length, ESplineCoordinateSpace::World);

	//@PlayerLocation + SplineLocation
	SplineLocation += PlayerLocation;

	//@Set Location
	SetActorLocation(SplineLocation);

	FRotator SplineRotation
		= Spline->GetRotationAtDistanceAlongSpline(fValue * length, ESplineCoordinateSpace::World);

	//@Set Rotation
	SetActorRotation(SplineRotation);
}
