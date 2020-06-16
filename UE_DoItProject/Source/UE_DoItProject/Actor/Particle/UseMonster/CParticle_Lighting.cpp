#include "CParticle_Lighting.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_HitComp.h"

ACParticle_Lighting::ACParticle_Lighting()
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

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Monster/Shaman/Shaman_FirstAttack_Lighting.Shaman_FirstAttack_Lighting'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> LightingRangeParticle(*strPath);
		if (LightingRangeParticle.Succeeded())
			ParticleComp->SetTemplate(LightingRangeParticle.Object);
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

	//TODO : Stun 으로 정의
	//DT_Burn = NewObject<UCDamageType_Burn>();
	//DT_Burn->SetSecondDamageValue(3.0f);
	//DT_Burn->SetBurnTime(5.0f);

#pragma endregion

}

void ACParticle_Lighting::BeginPlay()
{
	Super::BeginPlay();

	//@Tick 멈춤.
	SetActorTickEnabled(false);

	//@AddDynamic
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACParticle_Lighting::OnBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACParticle_Lighting::OnEndOverlap);
}

void ACParticle_Lighting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickTimer += DeltaTime;
	if (TickTimer > AttackRepeatTime)
	{
		TickTimer = 0.0f;
	}
}

void ACParticle_Lighting::OnStartActor(FVector Position)
{
	//@Tick ON
	SetActorTickEnabled(true);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ParticleComp->SetActive(true);
	SetActorLocation(Position);

	//@Begin Attack
	OnAttackingOverlap();
}

void ACParticle_Lighting::OffEndActor()
{
	//@Tick OFF
	SetActorTickEnabled(false);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleComp->SetActive(false);
}

void ACParticle_Lighting::SetBoxExtentScale(float fValue)
{
	CollisionBox->SetBoxExtent(FVector(fValue, fValue, 32.0f));
}

void ACParticle_Lighting::SetBoxExtentScale(FVector VecScale)
{
	CollisionBox->SetBoxExtent(FVector(VecScale.X, VecScale.Y, 32.0f));
}

void ACParticle_Lighting::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());

	bCollisioning = true;

	TArray<FOverlapResult> OverlapResults;
	FVector Position = GetActorLocation();
	FCollisionQueryParams Param(NAME_None, false, this);

	//@충돌 시행
	bool bOverlapValue = GetWorld()->OverlapMultiByChannel
	(
		OverlapResults, Position, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, //@Monster Attack
		FCollisionShape::MakeSphere(ActorOverlapSphereRadius),
		Param
	);

	if (bOverlapValue == true)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			IIC_Player* Player = Cast<IIC_Player>(OverlapResult.GetActor());
			if (Player != nullptr)
			{
				IIC_Charactor* Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
				if (Charactor != nullptr)
				{
					// 1. Get Interface HitComp
					IIC_HitComp* HitComp = Charactor->GetIHitComp();
					if (HitComp != nullptr)
					{
						//// 1.1 Set Hit Attribute
						FVector HitDir = OverlapResult.GetActor()->GetActorLocation() - GetActorLocation();
						HitDir.Z = 0.0f;
						HitDir.Normalize();
						HitComp->SetHitDirection(HitDir);

						// 1.2 Hit Delegate - DT_Burn
						HitComp->SetHitMoveSpeed(0.0f);
						HitComp->OnHit(this, DT_Stun, 5.0f);
					}
					else
						UE_LOG(LogTemp, Warning, L"Particle_FireRain BeginOverlap - HitComp Null!!");
				}
			}
		}//for(OverlapResult)
	}//(bOverlapValue == true)
}

void ACParticle_Lighting::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);
	IfTrueRet(OtherActor == GetOwner());

	bCollisioning = false;
}

void ACParticle_Lighting::OnAttackingOverlap()
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
			IIC_Player* Player = Cast<IIC_Player>(OverlapResult.GetActor());
			if (Player != nullptr)
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
						HitComp->SetHitMoveSpeed(2.0f);

						// 1.2 Hit Delegate -
						HitComp->OnHit(this, DT_Normal, 10.0f);
					}
					else
						UE_LOG(LogTemp, Warning, L"Particle_FireRain OnUpdateOverlap - HitComp Null!!");
				}
			}
		}//for(OverlapResult)
	}//(bOverlapValue == true)
}

