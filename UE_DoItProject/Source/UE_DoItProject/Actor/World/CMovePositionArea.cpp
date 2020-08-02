#include "CMovePositionArea.h"
#include "Global.h"

#include "Interface/IC_Player.h"

ACMovePositionArea::ACMovePositionArea()
{
	PrimaryActorTick.bCanEverTick = true;

	//@Create
	{
		SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
		RootComponent = SceneComp;

		BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
		BoxComp->SetupAttachment(RootComponent);

		SM_WarpDisplay = CreateDefaultSubobject<UStaticMeshComponent>("SM_Dispaly");
		SM_WarpDisplay->SetupAttachment(RootComponent);
	}

	//@Setting
	{
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetGenerateOverlapEvents(true);
		BoxComp->SetCollisionProfileName("OverlapOnlyPawn");
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f));

		SM_WarpDisplay->SetSimulatePhysics(false);
		SM_WarpDisplay->SetGenerateOverlapEvents(false);
		SM_WarpDisplay->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SM_WarpDisplay->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	FString strPath = L"";

	//@LOAD
	{
		//StaticMesh
		strPath = L"StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> DisplayMesh(*strPath);
		if (DisplayMesh.Succeeded())
		{
			SM_WarpDisplay->SetStaticMesh(DisplayMesh.Object);
		}

		//Material
		strPath = L"MaterialInstanceConstant'/Game/_Mine/UseMaterial/World/Mat_HologramGlowBlue_World.Mat_HologramGlowBlue_World'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat_Display(*strPath);
		if (Mat_Display.Succeeded())
		{
			SM_WarpDisplay->SetMaterial(0, Mat_Display.Object);
		}
	}

}

void ACMovePositionArea::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACMovePositionArea::OnBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACMovePositionArea::OnEndOverlap);
}

void ACMovePositionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACMovePositionArea::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);

	if (MovingAreaToActor != nullptr && bStartingPoint == true)
	{
		IIC_Player* I_Player = Cast<IIC_Player>(OtherActor);
		if(I_Player != nullptr)
		{
			OtherActor->SetActorLocation(MovingAreaToActor->GetActorLocation());
		}
	}
	else
	{
		CLog::Print(L"MovePositionArea AreaActor NULL!!");
		UE_LOG(LogTemp, Warning, L"MovePositionArea AreaActor NULL!!");
	}
}

void ACMovePositionArea::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);
}
