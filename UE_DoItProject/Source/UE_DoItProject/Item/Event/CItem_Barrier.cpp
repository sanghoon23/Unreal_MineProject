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
		ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Barrier(*strPath);
		if (SM_Barrier.Succeeded())
		{
			StaticMesh->SetStaticMesh(SM_Barrier.Object);
		}

		strPath = L"StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> MeshHologram(*strPath);
		if (MeshHologram.Succeeded())
		{
			SM_HologramGlow->SetStaticMesh(MeshHologram.Object);
		}
	}

	//@LOAD Particle
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_BarrierEffect.PS_BarrierEffect'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_Barrier(*strPath);
		if (P_Barrier.Succeeded())
		{
			ParticleBarrier = P_Barrier.Object;
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

	//@Create Ability
	{
		AbilityBarrier = NewObject<UCPLAbility_Barrier>();
	}
}

void ACItem_Barrier::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACItem_Barrier::OnBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACItem_Barrier::OnEnd);

	//@Setting Delegate
	{
		AbilityBarrier->OnEndTimerAbility.AddUObject(this, &ACItem_Barrier::DelegateAbilityEnd);

		//AbilityBarrier->OnEndTimerAbility.AddLambda([&](AActor*)
		//{
		//	CLog::Print(L"ParticleComp_Barrier Lambda IN!!");

		//	//@Particle OFF
		//	if (ParticleComp_Barrier != nullptr)
		//		ParticleComp_Barrier->SetActive(false);
		//	else CLog::Print(L"ParticleComp_Barrier NULL!!");
		//});
	}
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

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(EventedActor);
	if (I_Charactor != nullptr)
	{
		//@Collision OFF
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//@UnVisible
		SetActorHiddenInGame(true);

		//@파티클 실행
		{
			IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
			check(I_MeshParticle);

			//@Barrier Effect
			ParticleComp_Barrier = I_MeshParticle->SpawnParticleAtMesh
			(
				ParticleBarrier,
				EAttachPointType::BODY,
				EAttachPointRelative::NONE,
				EAttachLocation::SnapToTarget
			);
		}

		//@Ability 추가
		IIC_AbilityComp* I_AbilityComp = I_Charactor->GetIAbilityComp();
		if (I_AbilityComp != nullptr)
		{
			//@Create Ability
			FAbilityValue InputValue;
			InputValue.bTimer = true;
			InputValue.Timer = UsingAbilityTime;
			InputValue.Value = AddBarrierAmount;
			AbilityBarrier->SetAbilityValue(InputValue);

			AbilityBarrier->SetAppliedActor(EventedActor); //@Set Actor
			I_AbilityComp->AddAbility(AbilityBarrier);
		}
	}
}

void ACItem_Barrier::DelegateAbilityEnd(AActor* AppliedActor)
{
	check(AppliedActor);

	//@Particle OFF
	if (ParticleComp_Barrier != nullptr)
		ParticleComp_Barrier->SetActive(false);
	else CLog::Print(L"ParticleComp_Barrier NULL!!");

	//@DeathCall
	Death();
}
