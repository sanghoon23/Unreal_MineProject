#include "CItem_RecoveryMP.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_MeshParticle.h"
#include "Interface/IC_AbilityComp.h"

#include "Ability/Player/CPLAbility_MP.h"

ACItem_RecoveryMP::ACItem_RecoveryMP()
{
	PrimaryActorTick.bCanEverTick = true;

	//Super
	{
		ItemEventType = EItemEventType::RECOVERY_MP;
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

		SM_HologramGlow->SetSimulatePhysics(false);
		SM_HologramGlow->SetGenerateOverlapEvents(false);
		SM_HologramGlow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SM_HologramGlow->SetRelativeLocation(FVector(0.0f, 0.0f, 21.0f));
	}

	FString strPath = L"";

	//@LOAD Mesh
	{
		strPath = L"StaticMesh'/Game/_Mine/Mesh/Item/SM_RecoveryMP.SM_RecoveryMP'";
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
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_MP_HealingNotLoop.PS_MP_HealingNotLoop'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Healing(*strPath);
		if (PS_Healing.Succeeded())
		{
			HealingParticle = PS_Healing.Object;
		}
	}
}

void ACItem_RecoveryMP::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACItem_RecoveryMP::OnBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACItem_RecoveryMP::OnEnd);
}

void ACItem_RecoveryMP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@Y 축 회전
	FRotator Rotator = GetActorRotation();
	Rotator.Yaw += RotationValue * RotationSpeed * DeltaTime;
	SetActorRotation(Rotator);
}

void ACItem_RecoveryMP::OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;

	//@Event 적용
	ApplyEvent(OtherActor);
}

void ACItem_RecoveryMP::OnEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;
	if (GetOwner() == OtherActor) return;
}

void ACItem_RecoveryMP::ApplyEvent(AActor * EventedActor)
{
	Super::ApplyEvent(EventedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(EventedActor);
	if (I_Player != nullptr)
	{
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SetActorHiddenInGame(true);

		IIC_AbilityComp* I_AbilityComp = I_Player->GetIAbilityComp();
		if (I_AbilityComp != nullptr)
		{
			//@ADD Ability
			UCPLAbility_MP* Ability_MP = NewObject<UCPLAbility_MP>();
			FAbilityValue Input;
			Input.Value = RecoveryMPValue;
			Ability_MP->SetAbilityValue(Input);
			Ability_MP->SetAppliedActor(EventedActor);

			I_AbilityComp->AddAbility(Ability_MP);
		}

		//@Healing 파티클 실행
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(EventedActor);
		check(I_Charactor);
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		check(I_MeshParticle);

		//@Set Attach Particle -
		I_MeshParticle->SpawnParticleAtMesh
		(
			HealingParticle,
			EAttachPointType::ROOT,
			EAttachPointRelative::NONE,
			EAttachLocation::SnapToTargetIncludingScale
		);

		//@파괴
		FTimerHandle DeathTimerHandle;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACItem_RecoveryMP::Death, DeathTimeAfterRunning);

	}//(I_Player != nullptr)
}
