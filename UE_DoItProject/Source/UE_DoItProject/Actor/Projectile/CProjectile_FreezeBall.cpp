#include "CProjectile_FreezeBall.h"
#include "Global.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"

ACProjectile_FreezeBall::ACProjectile_FreezeBall()
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

		StaticMesh->SetRelativeScale3D(FVector(0.6f));
	}

	// @Paritcle Component
	{
		ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("ParticleComp");
		ParticleComp->SetupAttachment(SphereComp);

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Object/P_FreezeBall.P_FreezeBall'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> PT(*strPath);
		if (PT.Succeeded())
		{
			ParticleComp->SetTemplate(PT.Object);
		}

		ParticleComp->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
		ParticleComp->SetRelativeScale3D(FVector(0.3f));
	}
	#pragma endregion

	//@Explosion Particle
	strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Object/P_FreezeBallHit01.P_FreezeBallHit01'";
	ConstructorHelpers::FObjectFinder<UParticleSystem> PT_Explo(*strPath);
	if (PT_Explo.Succeeded())
	{
		P_ExplosionFreezeBall = PT_Explo.Object;
	}

	#pragma region Create DamageType
	//@Create DamageType
	DT_Freezing = NewObject<UCDamageType_Freeze>();
	DT_Freezing->SetFreezingTime(5.0f);

	#pragma endregion
}

void ACProjectile_FreezeBall::BeginPlay()
{
	Super::BeginPlay();
}

void ACProjectile_FreezeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// After delay seconds, call Function
	if (bSpawned == false)
	{
		bSpawned = true;
		FTimerHandle DeathTimerHandle;
		GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACProjectile_FreezeBall::Explosion, DeathTime);
	}

	if (FollowingTarget != nullptr)
	{
		UCFL_ActorAgainst::LookAtTarget(this, FollowingTarget);

		//@Target 이 움직일 수도 있어서 계산해주어야 함
		FVector TargetLocation = FollowingTarget->GetActorLocation();
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

void ACProjectile_FreezeBall::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);

	//@Following Target Check
	if (FollowingTarget != nullptr)
	{
		IfFalseRet(OtherActor == FollowingTarget);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FHitResult HitResult;

	FVector ActorForward = GetActorForwardVector();
	FVector Start = GetActorLocation();
	FVector End = GetActorLocation() + 10.0f;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(CollisionSphereRadius);
	FCollisionQueryParams Param(NAME_None, false, GetOwner());

	ECollisionChannel Channel;
	IIC_Charactor* const OwnerCharactor = Cast<IIC_Charactor>(GetOwner());
	(OwnerCharactor != nullptr)
		? Channel = OwnerCharactor->GetCharactorUsingAttackChannel()
		: Channel = ECollisionChannel::ECC_Visibility; //@Default

	//@충돌 시행
	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult, Start, End,
		FQuat::Identity,
		Channel, //@CharactorUsingChannel
		Sphere,
		Param
	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), End, Sphere.GetSphereRadius(), 40, FColor::Green, false, 0.1f);

#endif //  ENABLE_DRAW_DEBUG

	if (bHit == true)
	{
		CLog::Print(HitResult.GetActor()->GetName());

		//캐릭터 타입이 같지 않다면,
		IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
		if (Charactor != nullptr && OwnerCharactor != nullptr &&
			Charactor->GetCharactorType() != OwnerCharactor->GetCharactorType())
		{
			// 1. Get Interface HitComp
			IIC_HitComp* HitComp = Charactor->GetIHitComp();
			if (HitComp != nullptr)
			{
				// 1.1 Set Hit Attribute
				HitComp->SetHitDirection(FVector(0.0f));
				HitComp->SetHitMoveSpeed(0.0f);

				// 1.2 Hit Delegate - Normal(DamageType)
				HitComp->OnHit(this, DT_Freezing, 21.0f);
			}
			else
				UE_LOG(LogTemp, Warning, L"Projectile FreezeBall OnBeginOverlap - HitComp Null!!");

			//@폭발
			Explosion();

		}//(Interface Charactor)
	}//(bHit)
}

void ACProjectile_FreezeBall::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
}

void ACProjectile_FreezeBall::Explosion()
{
	//@터지는 파티클 실행
	FTransform P_Transform;
	P_Transform.SetLocation(GetActorLocation());
	P_Transform.SetScale3D(FVector(2.0f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_ExplosionFreezeBall, P_Transform, true);

	//@Projectile 파괴.
	Death();
}
