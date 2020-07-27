#include "CSkillRangeDisplay.h"
#include "Global.h"

ACSkillRangeDisplay::ACSkillRangeDisplay()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = SceneComp;

	PTComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComp");
	PTComp->SetupAttachment(RootComponent);

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
		PTComp->bAutoActivate = false;

		BoxComponent->SetCollisionProfileName("OverlapOnlyPawn");
		BoxComponent->SetGenerateOverlapEvents(true);
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		BackGroundStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ForwardStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//@반투명 정렬
		BackGroundStaticMesh->TranslucencySortPriority = 0;
		ForwardStaticMesh->TranslucencySortPriority = 1;
		PTComp->TranslucencySortPriority = 2;

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

void ACSkillRangeDisplay::BeginPlay()
{
	Super::BeginPlay();

	//Set Delegate
	{
		OnDelEndSkillCall.BindLambda([&]()
		{
			////@Particle OFF
			CLog::Print(L"Particle OFF!!");
			PTComp->SetActive(false);

			//@Init
			ForwardStaticMesh->SetWorldScale3D(FVector(0.0f, 0.0f, 1.0f));
			BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		});
	}

	//@BackgroudnStaticMesh 사용.
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACSkillRangeDisplay::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACSkillRangeDisplay::OnEndOverlap);
}

void ACSkillRangeDisplay::Tick(float DeltaTime)
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
			//BackGroundStaticMesh->SetVisibility(false);
			//ForwardStaticMesh->SetVisibility(false);
			SetVisibility(false);

			CLog::Print(L"Particle ON!!");
			PTComp->SetActive(true);

			//@Set Timer
			FTimerHandle DeathTimerHandle;
			GetWorldTimerManager().SetTimer
			(
				DeathTimerHandle,
				OnDelEndSkillCall,
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

void ACSkillRangeDisplay::SetVisibility(bool bValue)
{
	BackGroundStaticMesh->SetVisibility(bValue);
	ForwardStaticMesh->SetVisibility(bValue);
}

void ACSkillRangeDisplay::SetDecalCompMat(UMaterialInterface * Material, ESortType Type)
{
	check(Material);

	(Type == ESortType::BACK)
		? BackGroundStaticMesh->SetMaterial(0, Material)
		: ForwardStaticMesh->SetMaterial(0, Material);
}

void ACSkillRangeDisplay::SetBackGroundDecalSize(float fSize)
{
	BackGroundStaticMesh->SetWorldScale3D(FVector(fSize, fSize, 1.0f));
}

void ACSkillRangeDisplay::SetBackGroundDecalSize(FVector2D vSize)
{
	BackGroundStaticMesh->SetWorldScale3D(FVector(vSize.X, vSize.Y, 1.0f));
}

void ACSkillRangeDisplay::SetCollisionBoxExtent(FVector2D vSize)
{
	BoxComponent->SetBoxExtent(FVector(vSize.X, vSize.Y, DefaultBoxHeight));
}

void ACSkillRangeDisplay::SetParticleSystem(UParticleSystem * PT)
{
	check(PT);
	PTComp->SetTemplate(PT);
}

void ACSkillRangeDisplay::SetParticleSystem(UParticleSystem * PT, const FTransform& RelativeTransform)
{
	check(PT);
	PTComp->SetTemplate(PT);
	PTComp->SetRelativeTransform(RelativeTransform);
}

void ACSkillRangeDisplay::FillStart(AActor* SettingOwner, float fFillTimer, float fSkillTimer)
{
	check(SettingOwner);
	SetOwner(SettingOwner);

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetVisibility(true); //@BackGroundStaticMesh, FwdStaticMesh

	///////////////////////////////////////////////////////////////////////////////////
	bFilling = true;
	FillTimer = fFillTimer;
	SkillTimer = fSkillTimer;

	//Init
	ForwardStaticMesh->SetWorldScale3D(FVector(0.0f, 0.0f, 1.0f));
	FVector BackGroundSacle = BackGroundStaticMesh->GetRelativeScale3D();
	FVector ForwardScale = ForwardStaticMesh->GetRelativeScale3D();

	// 속력 = 거리 / 시간
	FillingSpeed = (BackGroundSacle.Y - ForwardScale.Y) / FillTimer;
}

void ACSkillRangeDisplay::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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

void ACSkillRangeDisplay::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());
}
