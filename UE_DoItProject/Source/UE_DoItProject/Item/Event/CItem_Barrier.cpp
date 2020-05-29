#include "CItem_Barrier.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_AbilityComp.h"
#include "Interface/IC_MeshParticle.h"

#include "Ability/Player/CPLAbility_Barrier.h"

ACItem_Barrier::ACItem_Barrier()
{
	PrimaryActorTick.bCanEverTick = true;

	//Super
	{
		ItemEventType = EItemEventType::BARRIER;
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
		BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 32.0f));
		//BoxComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetGenerateOverlapEvents(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));
		StaticMesh->SetRelativeScale3D(FVector(0.5f));

		SM_HologramGlow->SetSimulatePhysics(false);
		SM_HologramGlow->SetGenerateOverlapEvents(false);
		SM_HologramGlow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SM_HologramGlow->SetRelativeLocation(FVector(0.0f, 0.0f, 21.0f));
	}

	FString strPath = L"";

	//@LOAD Mesh
	{
		strPath = L"StaticMesh'/Game/_Mine/Mesh/Item/SM_Barrier.SM_Barrier'";
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
}

void ACItem_Barrier::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACItem_Barrier::OnBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACItem_Barrier::OnEnd);
}

void ACItem_Barrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACItem_Barrier::OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;

	//@Event 적용
	ApplyEvent(OtherActor);
}

void ACItem_Barrier::OnEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;
	if (GetOwner() == OtherActor) return;
}

void ACItem_Barrier::ApplyEvent(AActor * EventedActor)
{
	Super::ApplyEvent(EventedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(EventedActor);
	if (I_Player != nullptr)
	{
		//@Collision OFF
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//@UnVisible
		SetActorHiddenInGame(true);

		//@Ability 추가
		IIC_AbilityComp* I_AbilityComp = I_Player->GetIAbilityComp();
		if (I_AbilityComp != nullptr)
		{
			//@Create Ability
			UCPLAbility_Barrier* AbilityBarrier = NewObject<UCPLAbility_Barrier>();
			FAbilityValue InputValue;
			InputValue.bTimer = true;
			InputValue.Timer = 5.0f;
			InputValue.Value = AddBarrierAmount;
			AbilityBarrier->SetAbilityValue(InputValue);
			AbilityBarrier->SetAppliedActor(EventedActor); //@Set Actor

			I_AbilityComp->AddAbility(AbilityBarrier);
		}

		//@파괴
		FTimerHandle DeathTimerHandle;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACItem_Barrier::Death, DeathTimeAfterRunning);
	}
}
