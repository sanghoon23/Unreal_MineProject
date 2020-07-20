#include "CDecalActor_SkillRangeDisplay.h"
#include "Global.h"
#include "Components/DecalComponent.h"

ACDecalActor_SkillRangeDisplay::ACDecalActor_SkillRangeDisplay()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = SceneComp;

	BackGroundStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("BackGroundStaticMesh");
	BackGroundStaticMesh->SetupAttachment(SceneComp);

	ForwardStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ForwardStaticMesh");
	ForwardStaticMesh->SetupAttachment(SceneComp);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	BoxComponent->SetupAttachment(SceneComp);

	FString strPath = L"";

	strPath = L"StaticMesh'/Engine/BasicShapes/Plane.Plane'";
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Plane(*strPath);
	if (SM_Plane.Succeeded())
	{
		BackGroundStaticMesh->SetStaticMesh(SM_Plane.Object);
		ForwardStaticMesh->SetStaticMesh(SM_Plane.Object);
	}

	//@Set Material
	{
		strPath = L"Material'/Game/_Mine/UseMaterial/Decal/Mat_Decal_SkillRangeDisplay_Back.Mat_Decal_SkillRangeDisplay_Back'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> LoadedMat_Back(*strPath);
		if (LoadedMat_Back.Succeeded())
		{
			Mat_BackGround = LoadedMat_Back.Object;
		}

		strPath = L"Material'/Game/_Mine/UseMaterial/Decal/Mat_Decal_SkillRangeDisplay_Fwd.Mat_Decal_SkillRangeDisplay_Fwd'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> LoadedMat_Fwd(*strPath);
		if (LoadedMat_Fwd.Succeeded())
		{
			Mat_Forward = LoadedMat_Fwd.Object;
		}
	}

	//@Setting Value
	{
		BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
		BoxComponent->SetGenerateOverlapEvents(true);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		BackGroundStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ForwardStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//@반투명 정렬
		BackGroundStaticMesh->TranslucencySortPriority = 1;
		ForwardStaticMesh->TranslucencySortPriority = 2;

		//@Default Setting
		{
			SceneComp->SetRelativeLocation(FVector(0.0f, 0.0f, 30.0f));

			BackGroundStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
			BackGroundStaticMesh->SetWorldScale3D(FVector(5.0f, 5.0f, 1.0f));

			ForwardStaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
			ForwardStaticMesh->SetWorldScale3D(FVector(0.0f, 0.0f, 1.0f));

			BoxComponent->SetBoxExtent(FVector(125.0f, 125.0f, DefaultBoxHeight));
		}

		SetDecalCompMat(Mat_BackGround, ESortType::BACK);
		SetDecalCompMat(Mat_Forward, ESortType::FWD);
	}
}

void ACDecalActor_SkillRangeDisplay::BeginPlay()
{
	Super::BeginPlay();

	//@BackgroudnStaticMesh 사용.
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACDecalActor_SkillRangeDisplay::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACDecalActor_SkillRangeDisplay::OnEndOverlap);
}

void ACDecalActor_SkillRangeDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//#ForwardDecalComp Size 늘리기
	if (bFilling == true)
	{
		FVector BackGroundScale = BackGroundStaticMesh->GetRelativeScale3D();
		FVector ForwardScale = ForwardStaticMesh->GetRelativeScale3D();
		if (BackGroundScale.Y <= ForwardScale.Y)
		{
			bFilling = false;

			//@Collision
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

			//@Visible
			BackGroundStaticMesh->SetVisibility(false);
			ForwardStaticMesh->SetVisibility(false);

			//@Set Timer
			FTimerHandle DeathTimerHandle;
			FTimerDelegate CallOnDelAfterFill;
			CallOnDelAfterFill.BindLambda([&]() { Destroy(); }); //@Death Call
			GetWorldTimerManager().SetTimer
			(
				DeathTimerHandle,
				CallOnDelAfterFill,
				SkillTimer,
				false
			);

			return;
		}

		ForwardScale.X += (DeltaTime * FillingSpeed);
		ForwardScale.Y += (DeltaTime * FillingSpeed);

		ForwardStaticMesh->SetRelativeScale3D(ForwardScale);
	}
}

void ACDecalActor_SkillRangeDisplay::SetDecalCompMat(UMaterialInterface * Material, ESortType Type)
{
	check(Material);

	(Type == ESortType::BACK)
		? BackGroundStaticMesh->SetMaterial(0, Material)
		: ForwardStaticMesh->SetMaterial(0, Material);
}

void ACDecalActor_SkillRangeDisplay::SetBackGroundDecalSize(float fSize)
{
	BackGroundStaticMesh->SetWorldScale3D(FVector(fSize, fSize, 1.0f));
}

void ACDecalActor_SkillRangeDisplay::SetBackGroundDecalSize(FVector2D vSize)
{
	BackGroundStaticMesh->SetWorldScale3D(FVector(vSize.X, vSize.Y, 1.0f));
}

void ACDecalActor_SkillRangeDisplay::SetCollisionBoxExtent(FVector2D vSize)
{
	BoxComponent->SetBoxExtent(FVector(vSize.X, vSize.Y, DefaultBoxHeight));
}

void ACDecalActor_SkillRangeDisplay::FillStart(AActor* Owner, float fFillTimer, float fSkillTimer)
{
	check(Owner);
	SetOwner(Owner);

	bFilling = true;
	FillTimer = fFillTimer;
	SkillTimer = fSkillTimer;

	FVector BackGroundSacle = BackGroundStaticMesh->GetRelativeScale3D();
	FVector ForwardScale = ForwardStaticMesh->GetRelativeScale3D();

	// 속력 = 거리 / 시간
	FillingSpeed = (BackGroundSacle.Y - ForwardScale.Y) / FillTimer;
}

void ACDecalActor_SkillRangeDisplay::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());

	CLog::Print(L"SkillRangeDisplay BeginOverlap!!");

	if (OnDelOverlapSkillRange.IsBound())
	{
		OnDelOverlapSkillRange.Broadcast(OtherActor);
	}
}

void ACDecalActor_SkillRangeDisplay::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());
}
