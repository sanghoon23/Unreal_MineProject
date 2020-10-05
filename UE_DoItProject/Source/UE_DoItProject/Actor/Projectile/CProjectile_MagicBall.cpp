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
	//SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile_MagicBall::OnBeginOverlap);
	//SphereComp->OnComponentEndOverlap.AddDynamic(this, &ACProjectile_MagicBall::OnEndOverlap);

	//@Hit Comp
	//SphereComp->OnComponentHit.AddDynamic(this, &ACProjectile_MagicBall::OnCompHit);
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

	//@Check SettingTarget - 
	//#Edit 0905
	/*
	처음엔 BeginOverlap 에서 처리했지만 만약 TargetActor 앞에 가까이 다른 Actor 가 존재한다면,
	Projectile 은 생성되자마자 CNS_SpawnProjectile 의 SettingTarget 이 설정되기도 전에 
	TargetActor 가 아닌 다른 Actor 와 BeginOverlap 되고
	SettingTarget == OtherActor 가 아니라는 전재가 깨지면서 순서가 난감해짐.
	때문에 Tick 으로 옮김.
	OnHitComponent 도 시도했지만 Physics 를 설정해주어야 했기에
	원래 구현하고자 했던 내용에서 더 플러스 @ 가 되어버림.

	** 결론적으로, Projectile 에선 BeginOverlap, EndOverlap, HitComponent 도 쓰지 않음

	*/
	CheckSettingTarget();
}

void ACProjectile_MagicBall::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == GetOwner());
	IfTrueRet(OtherActor == this);
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

void ACProjectile_MagicBall::OnCompHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	Super::OnCompHit(HitComponent, OtherActor, OtherComponent, NormalImpulse, Hit);
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

void ACProjectile_MagicBall::CheckSettingTarget()
{
	TArray<FOverlapResult> OverlapResults;
	const FVector Position = GetActorLocation();

	FCollisionShape Sphere = FCollisionShape::MakeSphere(CollisionSphereRadius);
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
		Sphere, //200.0f
		Param
	);

	bool bCheckExistTarget = false;
	for (FOverlapResult& OverlapResult : OverlapResults)
	{
		if (OverlapResult.GetActor() == SettingTarget) bCheckExistTarget = true;
	}

	if (bOverlapValue == true && bCheckExistTarget == true)
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
					HitComp->OnHit(GetOwner(), DT_Normal, 10.0f);
				}
				else
					UE_LOG(LogTemp, Warning, L"ACProjectile MagicBall CallAttack - HitComp Null!!");
			}

			//@폭발
			Explosion();

		}//for(OverlapResult)

	}//(bOverlapValue == true)
}
