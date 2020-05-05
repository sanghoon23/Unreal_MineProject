#include "CParticle_FireRain.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"

ACParticle_FireRain::ACParticle_FireRain()
{
	PrimaryActorTick.bCanEverTick = true;

	FString strPath = L"";

	#pragma region Create Component
	// @Create BoxComponent
	{
		CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
		RootComponent = CollisionBox;
	}

	// Create Particle
	{
		ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>("FireRangeParticleComp");
		ParticleComp->SetupAttachment(CollisionBox);

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Attack/P_MGAttack_FireRange.P_MGAttack_FireRange'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> FireRangeParticle(*strPath);
		if (FireRangeParticle.Succeeded())
			ParticleComp->SetTemplate(FireRangeParticle.Object);
	}

	#pragma endregion

	//@Setting
	{
		CollisionBox->SetGenerateOverlapEvents(true);
		CollisionBox->SetCollisionProfileName("OverlapOnlyPawn");
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionBox->SetBoxExtent(FVector(200.0f, 200.0f, 32.0f));
		CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 16.0f));

		ParticleComp->bAutoActivate = false;
		ParticleComp->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
	}

	#pragma region DamageType

	DT_Normal = NewObject<UCDamageType_Normal>();

	DT_Burn = NewObject<UCDamageType_Burn>();
	DT_Burn->SetBurnTime(5.0f);

	#pragma endregion

}

void ACParticle_FireRain::BeginPlay()
{
	Super::BeginPlay();

	//@Tick 멈춤.
	SetActorTickEnabled(false);

	//@AddDynamic
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACParticle_FireRain::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACParticle_FireRain::OnEndOverlap);
}

void ACParticle_FireRain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickTimer += DeltaTime;
	if (TickTimer > AttackRepeatTime)
	{
		TickTimer = 0.0f;

		//@계속 겹침이 일어날 때, Update
		OnAttackingOverlap();
	}
}

void ACParticle_FireRain::OnStartActor(FVector Position)
{
	//@Tick ON
	SetActorTickEnabled(true);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ParticleComp->SetActive(true);
	SetActorLocation(Position);

	//@Begin Attack
	OnAttackingOverlap();
}

void ACParticle_FireRain::OffEndActor()
{
	//@Tick OFF
	SetActorTickEnabled(false);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleComp->SetActive(false);
}

void ACParticle_FireRain::SetBoxExtentScale(float fValue)
{
	CollisionBox->SetBoxExtent(FVector(fValue, fValue, 32.0f));
}

void ACParticle_FireRain::SetBoxExtentScale(FVector VecScale)
{
	CollisionBox->SetBoxExtent(FVector(VecScale.X, VecScale.Y, 32.0f));
}

void ACParticle_FireRain::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());

	CLog::Print(L"Begin Overlap");
	bCollisioning = true;

	TArray<FOverlapResult> OverlapResults;
	FVector Position = GetActorLocation();
	FCollisionQueryParams Param(NAME_None, false, this);

	//@충돌 시행
	bool bOverlapValue = GetWorld()->OverlapMultiByChannel
	(
		OverlapResults, Position, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, //@Player Attack
		FCollisionShape::MakeSphere(ActorOverlapSphereRadius),
		Param
	);

	if (bOverlapValue == true)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			//CLog::Print(OverlapResult.GetActor()->GetName());

			IIC_Monster* Monster = Cast<IIC_Monster>(OverlapResult.GetActor());
			if (Monster != nullptr)
			{
				IIC_Charactor* Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
				if (Charactor != nullptr)
				{
					// 1. Get Interface HitComp
					IIC_HitComp* HitComp = Charactor->GetIHitComp();
					if (HitComp != nullptr)
					{
						//// 1.1 Set Hit Attribute
						HitComp->SetHitDirection(FVector(0.0f));

						// 1.2 Hit Delegate - Air(DamageType)
						HitComp->SetHitMoveSpeed(0.0f);
						HitComp->OnHit(this, DT_Burn, 50.0f);
					}
					else
						UE_LOG(LogTemp, Warning, L"Particle_FireRain BeginOverlap - HitComp Null!!");
				}
			}
		}//for(OverlapResult)
	}//(bOverlapValue == true)
}

void ACParticle_FireRain::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());

	bCollisioning = false;
}

void ACParticle_FireRain::OnAttackingOverlap()
{
	TArray<FOverlapResult> OverlapResults;
	FVector Position = GetActorLocation();
	FCollisionQueryParams Param(NAME_None, false, this);

	//@충돌 시행
	bool bOverlapValue = GetWorld()->OverlapMultiByChannel
	(
		OverlapResults, Position, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2, //@Player Attack
		FCollisionShape::MakeSphere(ActorOverlapSphereRadius),
		Param
	);

	if (bOverlapValue == true)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			IIC_Monster* Monster = Cast<IIC_Monster>(OverlapResult.GetActor());
			if (Monster != nullptr)
			{
				IIC_Charactor* Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
				if (Charactor != nullptr)
				{
					// 1. Get Interface HitComp
					IIC_HitComp* HitComp = Charactor->GetIHitComp();
					if (HitComp != nullptr)
					{
						// 1.1 Set Hit Attribute
						FVector HitDirection = OverlapResult.GetActor()->GetActorLocation() - GetActorLocation();
						HitDirection.Z = 0.0f;
						HitDirection.Normalize();
						HitComp->SetHitDirection(HitDirection);
						HitComp->SetHitMoveSpeed(0.1f);

						// 1.2 Hit Delegate - Air(DamageType)
						HitComp->OnHit(this, DT_Normal, 50.0f);
					}
					else
						UE_LOG(LogTemp, Warning, L"Particle_FireRain OnUpdateOverlap - HitComp Null!!");
				}
			}
		}//for(OverlapResult)
	}//(bOverlapValue == true)
}
