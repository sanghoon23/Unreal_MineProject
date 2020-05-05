#include "CBaseProjectile.h"
#include "Global.h"

/* MoveSpeed & Direction */
float ACBaseProjectile::MoveSpeed = 0.0f;
FVector ACBaseProjectile::Direction = FVector(0.0f);
AActor* ACBaseProjectile::SettingTarget = nullptr;

ACBaseProjectile::ACBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	//@Setting
	{
		FollowingTarget = SettingTarget;
	}
}

void ACBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	//@Overlap
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACBaseProjectile::OnBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ACBaseProjectile::OnEndOverlap);
}

void ACBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@자식에 Moving 존재
}

void ACBaseProjectile::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	IfTrueRet(OtherActor == this);

	bOverlap = true;
}

void ACBaseProjectile::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	IfNullRet(OverlappedComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComp);

	bOverlap = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ACBaseProjectile::Death()
{
	Destroy();
}

void ACBaseProjectile::SettingProjectile(FVector Dir, float Speed)
{
	Direction = Dir;
	MoveSpeed = Speed;
}

void ACBaseProjectile::SettingProjectile(AActor * FollowingActor, FVector Dir, float Speed)
{
	check(FollowingActor);
	SettingTarget = FollowingActor;
	Direction = Dir;
	MoveSpeed = Speed;
}

void ACBaseProjectile::SettingProjectile(const FProjectileData & Data)
{
	Direction = Data.Direction;
	MoveSpeed = Data.MoveSpeed;
	SettingTarget = Data.FollowingTarget;
}
