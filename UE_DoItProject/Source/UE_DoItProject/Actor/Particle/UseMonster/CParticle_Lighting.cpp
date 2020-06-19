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
		CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 32.0f));
		CollisionBox->SetRelativeLocation(FVector(0.0f, 0.0f, 16.0f));

		ParticleComp->bAutoActivate = false;
		ParticleComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		ParticleComp->SetRelativeScale3D(FVector(2.0f));
	}
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

	/* 이 엑터는 계속 유지되지 않음 그래서 AttackOverlap 을 쓰지 않음 */
}

void ACParticle_Lighting::OnStartActor(FVector Position)
{
	//@Tick ON
	SetActorTickEnabled(true);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ParticleComp->SetActive(true);
	SetActorLocation(Position);

	//@Begin Attack
	//OnAttackingOverlap();
}

void ACParticle_Lighting::OffEndActor()
{
	//@Tick OFF
	SetActorTickEnabled(false);

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ParticleComp->SetActive(false);

	CLog::Print(L"Lighting OffEndActor!!");
}

void ACParticle_Lighting::SetDamageType(UCDamageType_Base * UseBeginOverlap)
{
	check(UseBeginOverlap);
	DT_UseBeginOverlap = UseBeginOverlap;
}

void ACParticle_Lighting::SetBoxExtentScale(float fValue)
{
	CollisionBox->SetBoxExtent(FVector(fValue, fValue, 32.0f));
}

void ACParticle_Lighting::SetBoxExtentScale(FVector VecScale)
{
	CollisionBox->SetBoxExtent(FVector(VecScale.X, VecScale.Y, 32.0f));
}

void ACParticle_Lighting::SetParticleCompRelative(FTransform Transform)
{
	ParticleComp->SetRelativeLocation(Transform.GetLocation());
	ParticleComp->SetRelativeRotation(FRotator(Transform.GetRotation()));
	ParticleComp->SetRelativeScale3D(Transform.GetScale3D());
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
			//@Player 를 받음
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
						HitComp->OnHit
						(
							GetOwner(), 
							DT_UseBeginOverlap, DT_UseBeginOverlap->DamageImpulse
						);
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

