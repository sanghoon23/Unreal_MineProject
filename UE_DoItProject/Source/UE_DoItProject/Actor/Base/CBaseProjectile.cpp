#include "CBaseProjectile.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

ACBaseProjectile::ACBaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//@만약 타겟이 죽었다면, Death
	if (SettingTarget != nullptr)
	{
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(SettingTarget);
		if (I_Charactor != nullptr)
		{
			if (I_Charactor->IsDeath() == true)
			{
				Death();
			}
		}
	}

	//@자식에 Moving 존재
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

void ACBaseProjectile::OnCompHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	IfNullRet(HitComponent);
	IfNullRet(OtherActor);
	IfNullRet(OtherComponent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ACBaseProjectile::Death()
{
	//FTimerDelegate Delegate;
	//Delegate.BindLambda([&]()
	//{
	//});
	//
	//GetWorldTimerManager().SetTimer(DeathTimerHandle, Delegate, 5.0f, false);

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
