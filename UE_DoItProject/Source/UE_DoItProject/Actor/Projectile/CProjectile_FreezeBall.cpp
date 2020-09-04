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

		ParticleComp->bAutoActivate = true;
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
}

void ACProjectile_FreezeBall::BeginPlay()
{
	Super::BeginPlay();

#pragma region Create DamageType
	//@Create DamageType
	DT_Freeze = NewObject<UCDamageType_Freeze>();
	DT_Freeze->SetFreezingTime(FreezeingTime);

	////@Set Delegate - Start
	////'=' 값 캡처
	//DT_Freeze->OnLinkStartUpsetCondition.AddLambda([](AActor* AppliedActor)
	//{
	//	IIC_Monster* SubjectI_Monster = Cast<IIC_Monster>(AppliedActor);
	//	if (SubjectI_Monster != nullptr)
	//	{
	//		SubjectI_Monster->SetAIRunningPossible(false);
	//	}
	//});

	////'=' 값 캡처
	//DT_Freeze->OnLinkEndUpsetCondition.AddLambda([](AActor* AppliedActor)
	//{
	//	IIC_Monster* SubjectI_Monster = Cast<IIC_Monster>(AppliedActor);
	//	if (SubjectI_Monster != nullptr)
	//	{
	//		SubjectI_Monster->SetAIRunningPossible(true);
	//	}
	//});

	//DT_Freeze->OnLinkStartUpsetCondition.AddWeakLambda(this, [](AActor* AppliedActor)
	//{
	//	IIC_Monster* SubjectI_Monster = Cast<IIC_Monster>(AppliedActor);
	//	if (SubjectI_Monster != nullptr)
	//	{
	//		SubjectI_Monster->SetAIRunningPossible(false);
	//	}
	//});

	////'=' 값 캡처
	//DT_Freeze->OnLinkEndUpsetCondition.AddWeakLambda(this, [](AActor* AppliedActor)
	//{
	//	IIC_Monster* SubjectI_Monster = Cast<IIC_Monster>(AppliedActor);
	//	if (SubjectI_Monster != nullptr)
	//	{
	//		SubjectI_Monster->SetAIRunningPossible(true);
	//	}
	//});

	DT_Freeze->OnLinkStartUpsetCondition.AddUObject(this, &ACProjectile_FreezeBall::FreezeStartDel);
	DT_Freeze->OnLinkEndUpsetCondition.AddUObject(this, &ACProjectile_FreezeBall::FreezeEndDel);

#pragma endregion
}

//#Edit 0708 - @Warning - CNS_SpawnProjectile 로 생성되어져 나가는 것을 기억해라.
void ACProjectile_FreezeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// After delay seconds, call Function
	if (bSpawned == false)
	{
		bSpawned = true;
		FTimerHandle DeathTimerHandle;
		//GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACProjectile_FreezeBall::Explosion, DeathTime);
	}

	if (SettingTarget != nullptr)
	{
		UCFL_ActorAgainst::LookAtTarget(this, SettingTarget);

		//@Target 이 움직일 수도 있어서 계산해주어야 함
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

	/* CProjectile_MagicBall 참조, */
	CheckSettingTarget();
}

void ACProjectile_FreezeBall::Explosion()
{
	//@터지는 파티클 실행
	FTransform P_Transform;
	P_Transform.SetLocation(GetActorLocation());
	P_Transform.SetScale3D(FVector(2.0f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_ExplosionFreezeBall, P_Transform, true);
}

void ACProjectile_FreezeBall::FreezeStartDel(AActor * Subject)
{
	IIC_Monster* SubjectI_Monster = Cast<IIC_Monster>(Subject);
	if (SubjectI_Monster != nullptr)
	{
		SubjectI_Monster->SetAIRunningPossible(false);
	}
}

void ACProjectile_FreezeBall::FreezeEndDel(AActor * Subject)
{
	IIC_Monster* SubjectI_Monster = Cast<IIC_Monster>(Subject);
	if (SubjectI_Monster != nullptr)
	{
		SubjectI_Monster->SetAIRunningPossible(true);
	}

	//@Projectile 파괴.
	Death();
}

void ACProjectile_FreezeBall::CheckSettingTarget()
{
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
	//TArray<FHitResult> HitResults;
	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel
	(
		HitResult, Start, End,
		FQuat::Identity,
		Channel, //@CharactorUsingChannel
		Sphere, //100.0f
		Param
	);

	if (bHit == true)
	{
		//@SettingTarget 과 다르다면,
		if (HitResult.GetActor() != SettingTarget) return;

		//캐릭터 타입이 같지 않다면,
		IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
		if (Charactor != nullptr && OwnerCharactor != nullptr &&
			Charactor->GetCharactorType() != OwnerCharactor->GetCharactorType())
		{
			// 1. Get Interface HitComp
			IIC_HitComp* HitComp = Charactor->GetIHitComp();
			if (HitComp != nullptr)
			{
				//@Particle OFF
				ParticleComp->SetVisibility(false);

				//@NoCollision
				SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				//@None Running Tick
				SetActorTickEnabled(false);

				// 1.1 Set Hit Attribute
				HitComp->SetHitDirection(FVector(0.0f));
				HitComp->SetHitMoveSpeed(0.0f);

				// 1.2 Hit Delegate - BeginPlay(DT_Freeze ADD Delegate)
				HitComp->OnHit(this, DT_Freeze, 20.0f);
			}
			else
				UE_LOG(LogTemp, Warning, L"Projectile FreezeBall OnBeginOverlap - HitComp Null!!");

			//@폭발
			Explosion();

		}//(Interface Charactor)
	}//(bHit)
}

