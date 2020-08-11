#include "CProjectile_MagicBall.h"
#include "Global.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"

ACProjectile_MagicBall::ACProjectile_MagicBall()
{
	PrimaryActorTick.bCanEverTick = true;

	FString strPath = L"";

	#pragma region Create Component
	// @SphereComp Component
	{
		SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
		RootComponent = SphereComp;

		SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SphereComp->SetGenerateOverlapEvents(true);
		SphereComp->SetCollisionProfileName("Projectile");
		SphereComp->SetVisibility(true);

		SphereComp->SetRelativeScale3D(FVector(2.0f));
	}

	// @StaticMesh Component
	{
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
		StaticMesh->SetupAttachment(SphereComp);

		strPath = L"StaticMesh'/Game/_Mine/Mesh/Object/Projectile/SM_ProjectileSpehre.SM_ProjectileSpehre'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> SM(*strPath);
		if (SM.Succeeded())
		{
			StaticMesh->SetStaticMesh(SM.Object);
		}

		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetEnableGravity(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMesh->SetVisibility(false);

		StaticMesh->SetRelativeScale3D(FVector(0.4f));

	}

	// @Paritcle Component
	{
		ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComp");
		ParticleComp->SetupAttachment(SphereComp);

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Object/P_MagicBall.P_MagicBall'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> PT(*strPath);
		if (PT.Succeeded())
		{
			ParticleComp->SetTemplate(PT.Object);
		}

		ParticleComp->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
		ParticleComp->SetRelativeScale3D(FVector(1.0f));
	}

	#pragma endregion

	//@Explosion Particle
	strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Object/P_MagicBallHit.P_MagicBallHit'";
	ConstructorHelpers::FObjectFinder<UParticleSystem> PT_Explo(*strPath);
	if (PT_Explo.Succeeded())
	{
		P_ExplosionMagicBall = PT_Explo.Object;
	}

	//@Create DamageType
	DT_Normal = NewObject<UCDamageType_Normal>();
}

void ACProjectile_MagicBall::BeginPlay()
{
	Super::BeginPlay();

	//@Overlap
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile_MagicBall::OnBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ACProjectile_MagicBall::OnEndOverlap);
}

//#Edit 0708 - @Warning - CNS_SpawnProjectile 로 생성되어져 나가는 것을 기억해라.
void ACProjectile_MagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// After delay seconds, call Function
	if (bSpawned == false)
	{
		bSpawned = true;
		FTimerHandle DeathTimerHandle;
		//GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACProjectile_MagicBall::Death, DeathTime);
	}

	if (SettingTarget != nullptr)
	{
		UCFL_ActorAgainst::LookAtTarget(this, SettingTarget);

		FVector TargetLocation = SettingTarget->GetActorLocation();
		FVector Location = GetActorLocation();
		Direction = TargetLocation - Location;
		Direction.Normalize();

		//@Set Location
		Location += Direction * MoveSpeed * DeltaTime;
		SetActorLocation(Location);
	}
	else
	{
		//@Moving
		FVector Location = GetActorLocation();
		Direction.Normalize();
		Location += Direction * MoveSpeed * DeltaTime;

		//@Set Location
		SetActorLocation(Location);
	}

}

void ACProjectile_MagicBall::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == GetOwner());
	IfTrueRet(OtherActor == this);

	//@Following Target Check
	if (SettingTarget != nullptr)
	{
		IfFalseRet(OtherActor == SettingTarget);
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	TArray<FOverlapResult> OverlapResults;
	const FVector Position = GetActorLocation();
	FCollisionQueryParams Param(NAME_None, false, GetOwner());


	//#Edit 0619 - I_Charactor ->GetUsingChannel 생성,
	ECollisionChannel Channel;
	IIC_Charactor* const OwnerCharactor = Cast<IIC_Charactor>(GetOwner());
	(OwnerCharactor != nullptr)
		? Channel = OwnerCharactor->GetCharactorUsingAttackChannel()
		: Channel = ECollisionChannel::ECC_Visibility; //@Default

	//@충돌 시행
	bool bOverlapValue = GetWorld()->OverlapMultiByChannel
	(
		OverlapResults, Position, FQuat::Identity,
		Channel, //@CharactorUsingChannel
		FCollisionShape::MakeSphere(CollisionSphereRadius),
		Param
	);

	if (bOverlapValue == true)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			//캐릭터 타입이 같지 않다면,
			IIC_Charactor* Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
			if (Charactor != nullptr && OwnerCharactor != nullptr &&
				Charactor->GetCharactorType() != OwnerCharactor->GetCharactorType())
			{
				// 1. Get Interface HitComp
				IIC_HitComp* HitComp = Charactor->GetIHitComp();
				if (HitComp != nullptr)
				{
					// 1.1 Set Hit Attribute
					FVector ActorLocation = GetActorLocation();
					FVector HitDirection = OverlapResult.GetActor()->GetActorLocation() - ActorLocation;
					HitDirection.Normalize();
					HitDirection.Z = 0.0f;
					HitComp->SetHitDirection(HitDirection);
					HitComp->SetHitMoveSpeed(0.3f);

					// 1.2 Hit Delegate - Air(DamageType)
					HitComp->OnHit(this, DT_Normal, 10.0f);
				}
				else
					UE_LOG(LogTemp, Warning, L"ACProjectile MagicBall CallAttack - HitComp Null!!");
			}
		}//for(OverlapResult)

		//@폭발
		Explosion();

	}//(bOverlapValue == true)
}

void ACProjectile_MagicBall::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == GetOwner());
	IfTrueRet(OtherActor == this);
}

void ACProjectile_MagicBall::Explosion()
{
	//@NoCollision
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//@None Running Actor Tick
	SetActorTickEnabled(false);

	//@Visible
	ParticleComp->SetActive(false);

	//@터지는 파티클 실행
	FTransform P_Transform;
	P_Transform.SetLocation(GetActorLocation());
	P_Transform.SetScale3D(FVector(2.0f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_ExplosionMagicBall, P_Transform, true);

	//@Projectile 파괴.
	Death();
}
