#include "CItem_Faster.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_AbilityComp.h"
#include "Interface/IC_MeshParticle.h"

#include "Ability/Player/CPLAbility_SpeedUpper.h"

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
		BoxComp->SetBoxExtent(FVector(50.0f, 50.0f, 32.0f));
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

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_ApplyFasterWithHand.PS_ApplyFasterWithHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_LHandSpeedUp(*strPath);
		if (P_LHandSpeedUp.Succeeded())
		{
			SpeedUp_LHand = P_LHandSpeedUp.Object;
		}

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_ApplyFasterWithHand.PS_ApplyFasterWithHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_RHandSpeedUp(*strPath);
		if (P_RHandSpeedUp.Succeeded())
		{
			SpeedUp_RHand = P_RHandSpeedUp.Object;
		}
	}

	//@Create Ability
	{
		AbilitySpeedUpper = NewObject<UCPLAbility_SpeedUpper>();
	}
}

void ACItem_Faster::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACItem_Faster::OnBegin);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACItem_Faster::OnEnd);

	//@Setting Ability
	{
		////@Set Delegate - Start
		//AbilitySpeedUpper->OnDelStartTimerAbility.AddLambda([&](AActor* AppliedActor)
		//{
		//	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
		//	if (I_Player != nullptr)
		//	{
		//		//@기존 Player Particle OFF
		//		I_Player->OffParticleInPlayer();
		//	}
		//});

		////@Set Delegate - End
		//AbilitySpeedUpper->OnEndTimerAbility.AddLambda([&](AActor* AppliedActor)
		//{
		//	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
		//	if (I_Player != nullptr)
		//	{
		//		//@기존 Player Particle ON
		//		I_Player->OnParticleInPlayer();
		//	}

		//	//@Particle OFF
		//	if (ParticleComp_LHand != nullptr)
		//		ParticleComp_LHand->SetActive(false);
		//	else CLog::Print(L"LHand Particle NULL!!");

		//	if (ParticleComp_RHand != nullptr)
		//		ParticleComp_RHand->SetActive(false);
		//	else CLog::Print(L"RHand Particle NULL!!");
		//});

		AbilitySpeedUpper->OnDelStartTimerAbility.AddUObject(this, &ACItem_Faster::DelegateAbilityStart);
		AbilitySpeedUpper->OnEndTimerAbility.AddUObject(this, &ACItem_Faster::DelegateAbilityEnd);
	}
}

void ACItem_Faster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@Y 축 회전
	FRotator Rotator = GetActorRotation();
	Rotator.Yaw += RotationValue * RotationSpeed * DeltaTime;
	SetActorRotation(Rotator);
}

void ACItem_Faster::OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;

	//@Event 적용
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

			//@Faster Effect
			I_MeshParticle->SpawnParticleAtMesh
			(
				FasterParticle,
				EAttachPointType::ROOT,
				EAttachPointRelative::NONE,
				EAttachLocation::SnapToTarget
			);

			//@LHand ParticleComp
			ParticleComp_LHand = I_MeshParticle->SpawnParticleAtMesh
			(
				SpeedUp_LHand,
				EAttachPointType::LHAND,
				EAttachPointRelative::RELATIVE,
				EAttachLocation::SnapToTarget
			);

			//@RHand ParticleComp
			ParticleComp_RHand = I_MeshParticle->SpawnParticleAtMesh
			(
				SpeedUp_RHand,
				EAttachPointType::RHAND,
				EAttachPointRelative::RELATIVE,
				EAttachLocation::SnapToTarget
			);
		}

		//@Ability 추가
		IIC_AbilityComp* I_AbilityComp = I_Charactor->GetIAbilityComp();
		if (I_AbilityComp != nullptr)
		{
			FAbilityValue InputValue;
			InputValue.bTimer = true;
			InputValue.Timer = UsingAbilitySpeedUpperTime;
			InputValue.Value = AddSpeedValue;
			AbilitySpeedUpper->SetAbilityValue(InputValue);

			AbilitySpeedUpper->SetAppliedActor(EventedActor);
			I_AbilityComp->AddAbility(AbilitySpeedUpper);
		}

		////@파괴 - SetTimer 호출
		//FTimerHandle DeathTimerHandle;
		//GetWorldTimerManager().SetTimer
		//(
		//	DeathTimerHandle, this, &ACItem_Faster::Death, 
		//	DeathTimeAfterRunning + 0.5f //@ + Offset
		//);

	}//(I_Player != nullptr)

}

void ACItem_Faster::DelegateAbilityStart(AActor* AppliedActor)
{
	check(AppliedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@기존 Player Particle OFF
		I_Player->OffParticleInPlayer();
	}
}

void ACItem_Faster::DelegateAbilityEnd(AActor* AppliedActor)
{
	check(AppliedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@기존 Player Particle ON
		I_Player->OnParticleInPlayer();
	}

	//@Particle OFF
	if (ParticleComp_LHand != nullptr)
		ParticleComp_LHand->SetActive(false);
	else CLog::Print(L"LHand Particle NULL!!");

	if (ParticleComp_RHand != nullptr)
		ParticleComp_RHand->SetActive(false);
	else CLog::Print(L"RHand Particle NULL!!");

	//@Death Call
	Death();
}
