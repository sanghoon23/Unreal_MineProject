#include "CItem_Faster.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_MeshParticle.h"

ACItem_Faster::ACItem_Faster()
{
	PrimaryActorTick.bCanEverTick = true;

	//Super
	{
		ItemEventType = EItemEventType::FASTER;
	}

	//@Create Component
	{
		BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
		RootComponent = BoxComp;

		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
		StaticMesh->SetupAttachment(BoxComp);

		SM_HologramGlow = CreateDefaultSubobject<UStaticMeshComponent>("Hologram");
		SM_HologramGlow->SetupAttachment(BoxComp);
	}

	//@Setting
	{
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetGenerateOverlapEvents(true);
		BoxComp->SetCollisionProfileName("OverlapOnlyPawn");
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetGenerateOverlapEvents(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
		StaticMesh->SetRelativeScale3D(FVector(0.8f));

		SM_HologramGlow->SetSimulatePhysics(false);
		SM_HologramGlow->SetGenerateOverlapEvents(false);
		SM_HologramGlow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SM_HologramGlow->SetRelativeLocation(FVector(0.0f, 0.0f, 21.0f));
	}

	FString strPath = L"";

	//@LOAD Mesh
	{
		strPath = L"StaticMesh'/Game/_Mine/Mesh/Item/SM_Faster.SM_Faster'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> SM_RecoveryHp(*strPath);
		if (SM_RecoveryHp.Succeeded())
		{
			StaticMesh->SetStaticMesh(SM_RecoveryHp.Object);
		}

		strPath = L"StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> MeshHologram(*strPath);
		if (MeshHologram.Succeeded())
		{
			SM_HologramGlow->SetStaticMesh(MeshHologram.Object);
		}
	}

	//@LOAD Material
	{
		strPath = L"MaterialInstanceConstant'/Game/_Mine/UseMaterial/Item/Mat_HologramGlowGreen.Mat_HologramGlowGreen'";
		ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat_Hologram(*strPath);
		if (Mat_Hologram.Succeeded())
		{
			SM_HologramGlow->SetMaterial(0, Mat_Hologram.Object);
		}
	}

	//@LOAD Particle
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_FasterEffect.PS_FasterEffect'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Faster(*strPath);
		if (PS_Faster.Succeeded())
		{
			FasterParticle = PS_Faster.Object;
		}
	}
}

void ACItem_Faster::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACItem_Faster::OnBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACItem_Faster::OnEnd);
}

void ACItem_Faster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@Y �� ȸ��
	FRotator Rotator = GetActorRotation();
	Rotator.Yaw += RotationValue * RotationSpeed * DeltaTime;
	SetActorRotation(Rotator);
}

void ACItem_Faster::OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;

	//@Event ����
	ApplyEvent(OtherActor);
}

void ACItem_Faster::OnEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;
	if (GetOwner() == OtherActor) return;
}

void ACItem_Faster::ApplyEvent(AActor * EventedActor)
{
	Super::ApplyEvent(EventedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(EventedActor);
	if (I_Player != nullptr)
	{
		//@Collision OFF
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//@UnVisible
		SetActorHiddenInGame(true);

		I_Player->SpeedUp(AddSpeedValue);

		//@Healing ��ƼŬ ����
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(EventedActor);
		check(I_Charactor);

		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		check(I_MeshParticle);

		//@Set Attach Particle -
		I_MeshParticle->SpawnParticleAtMesh
		(
			FasterParticle,
			AttachPointType::BODY,
			EAttachLocation::SnapToTargetIncludingScale
		);

		//@�ı�
		FTimerHandle DeathTimerHandle;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACItem_Faster::Death, DeathTimeAfterRunning);

	}//(I_Player != nullptr)

}
