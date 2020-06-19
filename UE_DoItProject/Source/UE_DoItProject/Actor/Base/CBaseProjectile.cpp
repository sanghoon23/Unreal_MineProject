#include "CBaseProjectile.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

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

	//@���� Ÿ���� �׾��ٸ�, ����
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(SettingTarget);
	if (I_Charactor != nullptr)
	{
		if (I_Charactor->IsDeath() == true)
		{
			Death();
		}
	}

	//@�ڽĿ� Moving ����
	//...
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

void ACBaseProjectile::SettingDirection(FVector Dir)
{
	Direction = Dir;
}

void ACBaseProjectile::SettingSpeed(float Speed)
{
	MoveSpeed = Speed;
}

void ACBaseProjectile::SettingTargetActor(AActor * Target)
{
	SettingTarget = Target;
}
