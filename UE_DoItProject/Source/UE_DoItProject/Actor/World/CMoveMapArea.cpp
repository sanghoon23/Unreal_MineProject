#include "CMoveMapArea.h"
#include "Global.h"

#include "UMG.h"
#include "Interface/IC_Player.h"

ACMoveMapArea::ACMoveMapArea()
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

		//Background Texture
		strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/Backgournd/Bp_LoadingBackground_01.Bp_LoadingBackground_01_C'";
		ConstructorHelpers::FClassFinder<UUserWidget> LoadingClass(*strPath);
		if (LoadingClass.Succeeded())
		{
			LoadingBackgroundClass = LoadingClass.Class;
		}
	}

}

void ACMoveMapArea::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACMoveMapArea::OnBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACMoveMapArea::OnEndOverlap);
}

void ACMoveMapArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACMoveMapArea::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);

	IIC_Player* I_Player = Cast<IIC_Player>(OtherActor);
	if (I_Player != nullptr)
	{
		//@LOAD MAP
		CLog::Print(L"MoveMapAreaActor BeginOverlap!!");
		CLog::Print(LoadMapName);

		UWorld* const World = OtherActor->GetWorld();

		//@Loading...
		UUserWidget* LoadBackground = CreateWidget<UUserWidget>(GetWorld(), LoadingBackgroundClass);
		if (LoadBackground != nullptr)
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0); //@ÁÖÃ¼ÀÚ.
			if (PC != nullptr)
			{
				LoadBackground->AddToViewport();
			}

			FTimerHandle OpenLevelTimerHandle;
			FTimerDelegate TimerDelegate;
			FName LambdaInsertLoadMapName = LoadMapName;
			TimerDelegate.BindLambda([LoadBackground, World, LambdaInsertLoadMapName]()
			{
				//@Remove
				LoadBackground->RemoveFromViewport();

				//@Open Level(MapName)
				UGameplayStatics::OpenLevel(World, LambdaInsertLoadMapName);
			});
			GetWorldTimerManager().SetTimer(OpenLevelTimerHandle, TimerDelegate, 3.0f, false);
		}
	}
}

void ACMoveMapArea::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);
}
