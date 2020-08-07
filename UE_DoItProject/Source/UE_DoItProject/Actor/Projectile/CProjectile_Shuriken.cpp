#include "CProjectile_Shuriken.h"
#include "Global.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_HitComp.h"

ACProjectile_Shuriken::ACProjectile_Shuriken()
{
	PrimaryActorTick.bCanEverTick = true;

	FString strPath = L"";

#pragma region Create Component
	//// @SphereComp Component
	//{
	//	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	//	RootComponent = SphereComp;

	//	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//	SphereComp->SetGenerateOverlapEvents(true);
	//	SphereComp->SetCollisionProfileName("Projectile");
	//	SphereComp->SetVisibility(true);

	//	SphereComp->SetRelativeScale3D(FVector(2.0f));
	//}

	////@Capsule Comp
	//{
	//	Capsule = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	//	RootComponent = Capsule;

	//	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//	Capsule->SetGenerateOverlapEvents(true);
	//	Capsule->SetCollisionProfileName("Projectile");
	//	Capsule->SetVisibility(true);
	//}

	// @StaticMesh Component
	{
		SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComp");
		RootComponent = SkeletalMesh;
		//SkeletalMesh->SetupAttachment(Capsule);

		strPath = L"SkeletalMesh'/Game/_Mine/Mesh/Object/Projectile/Blade_Fencer/SK_Blade_Fencer.SK_Blade_Fencer'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> SK(*strPath);
		if (SK.Succeeded())
		{
			SkeletalMesh->SetSkeletalMesh(SK.Object);
		}

		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkeletalMesh->SetGenerateOverlapEvents(true);
		SkeletalMesh->SetCollisionProfileName("Projectile");
		SkeletalMesh->SetVisibility(true);

		//Test Code
		//SkeletalMesh->SetSimulatePhysics(false);
		//SkeletalMesh->SetEnableGravity(false);
		//SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//SkeletalMesh->SetVisibility(false);
	}

#pragma endregion

	//@Explosion Particle
	strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Object/P_ShurikenPoisionExplo.P_ShurikenPoisionExplo'";
	ConstructorHelpers::FObjectFinder<UParticleSystem> PT_Explo(*strPath);
	if (PT_Explo.Succeeded())
	{
		P_ExplosionShuriken = PT_Explo.Object;
	}
}

void ACProjectile_Shuriken::BeginPlay()
{
	Super::BeginPlay();

	//@Overlap
	SkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile_Shuriken::OnBeginOverlap);
	SkeletalMesh->OnComponentEndOverlap.AddDynamic(this, &ACProjectile_Shuriken::OnEndOverlap);

#pragma region Create DamageType

	DT_Normal= NewObject<UCDamageType_Normal>();
	DT_Normal->SetDamageImpulse(20.0f);

	DT_Poision = NewObject<UCDamageType_Poision>();
	DT_Poision->SetSecondDamageValue(5.0f);
	DT_Poision->SetPoisioningTime(5.0f);

#pragma endregion
}

//#Edit 0708 - @Warning - CNS_SpawnProjectile 로 생성되어져 나가는 것을 기억해라.
void ACProjectile_Shuriken::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// After delay seconds, call Function
	if (bSpawned == false)
	{
		bSpawned = true;
		//FTimerHandle DeathTimerHandle;
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

}

void ACProjectile_Shuriken::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::OnBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);

	//@Following Target Check
	if (SettingTarget != nullptr)
	{
		IfFalseRet(OtherActor == SettingTarget);
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
		//캐릭터 타입이 같지 않다면,
		IIC_Charactor* Charactor = Cast<IIC_Charactor>(HitResult.GetActor());
		if (Charactor != nullptr && OwnerCharactor != nullptr &&
			Charactor->GetCharactorType() != OwnerCharactor->GetCharactorType())
		{
			// 1. Get Interface HitComp
			IIC_HitComp* HitComp = Charactor->GetIHitComp();
			if (HitComp != nullptr)
			{
				//@NoCollision
				SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				//@None Running Tick
				SetActorTickEnabled(false);

				// 1.1 Set Hit Attribute
				HitComp->SetHitDirection(FVector(0.0f));
				HitComp->SetHitMoveSpeed(0.0f);

				// 1.2 Hit Delegate - BeginPlay(DT_Freeze ADD Delegate)
				HitComp->OnHit(this, DT_Normal, DT_Normal->DamageImpulse);
			}
			else
				UE_LOG(LogTemp, Warning, L"Projectile FreezeBall OnBeginOverlap - HitComp Null!!");

			//@폭발
			Explosion();

		}//(Interface Charactor)
	}//(bHit)
}

void ACProjectile_Shuriken::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	Super::OnEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);
}

void ACProjectile_Shuriken::Explosion()
{
	//@터지는 파티클 실행
	FTransform P_Transform;
	P_Transform.SetLocation(GetActorLocation());
	P_Transform.SetScale3D(FVector(2.0f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), P_ExplosionShuriken, P_Transform, true);

	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ACProjectile_Shuriken::Explosion, DeathTime);
}
