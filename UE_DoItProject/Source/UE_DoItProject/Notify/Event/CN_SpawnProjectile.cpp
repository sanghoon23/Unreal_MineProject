#include "CN_SpawnProjectile.h"
#include "Global.h"

#include "Actor/Base/CBaseProjectile.h"
#include "Actor/Projectile/CProjectile_MagicBall.h"

void UCN_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	IfNullRet(MeshComp);
	IfNullRet(Animation);

	AActor* OwnerActor = Cast<AActor>(MeshComp->GetOwner());
	IfNullRet(OwnerActor);

///////////////////////////////////////////////////////////////////////////////////////////////////////////

	FTransform SpawnTransform = FTransform::Identity;
	SpawnTransform = OwnerActor->GetTransform();

	if (AttachName.GetStringLength() > 0)
	{
		//@이름이 있는지 체크
		if (MeshComp->DoesSocketExist(AttachName))
		{
			FVector BoneLocation = MeshComp->GetBoneLocation(AttachName, EBoneSpaces::WorldSpace);
			SpawnTransform.SetLocation(BoneLocation);
		}
	}

	//@Get
	FVector		SpawnLocation	= SpawnTransform.GetLocation();
	FRotator	SpawnRotation	= SpawnTransform.GetRotation().Rotator();
	FVector		SpawnScale		= SpawnTransform.GetScale3D();

	//@Calc
	SpawnLocation	+= LocationOffset;
	SpawnRotation	+= RotationOffset;
	SpawnScale		+= ScaleOffset;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(FQuat(SpawnRotation));
	SpawnTransform.SetScale3D(SpawnScale);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@Spawn
	UWorld* const World = MeshComp->GetWorld();
	if (SpawnProjectileClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"CN_SpawnProjectile UClass NULL!!");
		return;
	}

	FActorSpawnParameters Params;
	Params.Owner = OwnerActor;
	ACBaseProjectile* SpawnProjectile = World->SpawnActor<ACBaseProjectile>(SpawnProjectileClass, SpawnTransform, Params);
	if (SpawnProjectile != nullptr)
	{
		//@Set Value
		SpawnProjectile->SettingSpeed(SpeedValue);
		SpawnProjectile->SettingDirection(Direction);
		if (Target != nullptr)
		{
			//CLog::Print(Target->GetName());
			SpawnProjectile->SettingTargetActor(Target);
		}
	}
	else if(SpawnProjectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"CN_SpawnProjectile SpawnProjectile NULL!!");
	}
}

void UCN_SpawnProjectile::SetProjectileDirection(FVector Dir)
{
	Direction = Dir;
}

void UCN_SpawnProjectile::SetProjectileTarget(AActor * Actor)
{
	Target = Actor;
}
