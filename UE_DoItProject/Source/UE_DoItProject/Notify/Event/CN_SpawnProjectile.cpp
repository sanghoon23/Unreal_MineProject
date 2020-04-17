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
	FActorSpawnParameters Params;

	SpawnTransform = OwnerActor->GetTransform();
	Params.Owner = OwnerActor;

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

	//CLog::Print(SpawnRotation.ToString());

	//@Calc
	SpawnLocation	+= LocationOffset;
	SpawnRotation	+= RotationOffset;
	SpawnScale		+= ScaleOffset;

	//@Set
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(FQuat(SpawnRotation));
	SpawnTransform.SetScale3D(SpawnScale);

	/* 헤더 추가해주어야 함. */
	//@Spawn
	UWorld* const World = MeshComp->GetWorld();

	IfNullRet(SpawnProjectileClass);
	if (SpawnProjectileClass != nullptr)
	{
		World->SpawnActor<ACBaseProjectile>(SpawnProjectileClass, SpawnTransform, Params);
	}
	else
		UE_LOG(LogTemp, Warning, L"CN_SpawnProjectile UClass NULL!!");
}
