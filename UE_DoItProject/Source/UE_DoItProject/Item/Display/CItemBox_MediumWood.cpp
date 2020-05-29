#include "CItemBox_MediumWood.h"
#include "Global.h"

#include "_FunctionLibrary/CFL_ItemSpawning.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"

#include "Item/Event/CItem_RecoveryHP.h"
#include "Item/Event/CItem_RecoveryMP.h"
#include "Item/Event/CItem_Faster.h"
#include "Item/Event/CItem_Barrier.h"

ACItemBox_MediumWood::ACItemBox_MediumWood()
{
	PrimaryActorTick.bCanEverTick = true;

	//Super
	{
		ItemType = UItemType::NONE;
	}

	//@Create
	{
		BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
		RootComponent = BoxComp;

		SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
		SkeletalMesh->SetupAttachment(BoxComp);
	}

	//@Setting
	{
		// Hit - BoxComp->set
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetGenerateOverlapEvents(true);
		BoxComp->SetCollisionProfileName("BlockOnlyPLAttackTrace"); //@OverlapOnlyPLAttack
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BoxComp->SetBoxExtent(FVector(40.0f, 90.0f, 55.0f));

		SkeletalMesh->SetSimulatePhysics(false);
		SkeletalMesh->SetGenerateOverlapEvents(false);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
		SkeletalMesh->SetRelativeScale3D(FVector(2.0f));
	}


	#pragma region LOAD
	FString strPath = L"";

	//@LOAD Mesh
	{
		strPath = L"SkeletalMesh'/Game/_Mine/Mesh/Item/SK_ItemBox_MediumWood.SK_ItemBox_MediumWood'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> SKM_MediumWood(*strPath);
		if (SKM_MediumWood.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(SKM_MediumWood.Object);
		}
	}

	//@LOAD Particle
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Item/PS_ItemBoxOpening.PS_ItemBoxOpening'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_Opening(*strPath);
		if (P_Opening.Succeeded())
		{
			OpeningParticle = P_Opening.Object;
		}
	}

	//#Edit - AnimInst 로.
	////@LOAD Montage
	//{
	//	strPath = L"AnimMontage'/Game/_Mine/Montages/Environment/MediumWood_Mon_Open.MediumWood_Mon_Open'";
	//	ConstructorHelpers::FObjectFinder<UAnimMontage> MonOpening(*strPath);
	//	if (MonOpening.Succeeded())
	//	{
	//		OpeningMontage = MonOpening.Object;
	//	}
	//}

	#pragma endregion
}

void ACItemBox_MediumWood::BeginPlay()
{
	Super::BeginPlay();

	//BoxComp->OnComponentHit.AddDynamic(this, &ACItemBox_MediumWood::OnHit);

	//BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACItemBox_MediumWood::OnBegin);
	//BoxComp->OnComponentEndOverlap.AddDynamic(this, &ACItemBox_MediumWood::OnEnd);
}

void ACItemBox_MediumWood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACItemBox_MediumWood::OnBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;

	IIC_Player* I_Player = Cast<IIC_Player>(OtherActor);
	if (I_Player != nullptr)
	{
		CLog::Print(L"OnBegin ItemBox_MediumWood!!");
	}
}

void ACItemBox_MediumWood::OnEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;
	if (GetOwner() == OtherActor) return;
}

void ACItemBox_MediumWood::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
	if (OtherActor == this) return;

	CLog::Print(OtherActor->GetName());
}

float ACItemBox_MediumWood::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UWorld* const World = GetWorld();

	//@한번만 들어오게
	SetCanBeDamaged(false);

	bBoxOpening = true;

	//@생성
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACItemBox_MediumWood::SpawnItem, 1.0f);

	//@파티클
	FTransform Transform;
	Transform.SetLocation(GetActorLocation());
	Transform.SetScale3D(FVector(2.0f));
	UGameplayStatics::SpawnEmitterAtLocation(World, OpeningParticle, Transform, true);

	return Damage;
}

void ACItemBox_MediumWood::SpawnItem()
{
	//@OFF Collision
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//@Spawn
	UCFL_ItemSpawning::RandomSpawnAppliedItemEvent
	(
		this,
		EItemEventType::RECOVERY_HP,
		EItemEventType::BARRIER,
		FVector(GetActorLocation())
	);

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACItemBox_MediumWood::Death, DeathTimeAfterRunning);
}
