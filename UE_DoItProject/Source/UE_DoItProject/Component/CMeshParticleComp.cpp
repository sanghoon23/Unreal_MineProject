#include "CMeshParticleComp.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMeshParticleComp::UCMeshParticleComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}

void UCMeshParticleComp::BeginPlay()
{
	Super::BeginPlay();

	CharactorOwner = Cast<ACharacter>(GetOwner());
	check(CharactorOwner);
}

void UCMeshParticleComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UParticleSystemComponent* UCMeshParticleComp::SpawnParticleAtMesh(UParticleSystem * const PT, const EAttachPointType Point, const EAttachPointRelative Rel, const EAttachLocation::Type AttachLocation)
{
	check(PT);

	FName AttachName;
	const bool bResult = CheckAttackName(AttachName, Point, Rel);

	UWorld* const World = CharactorOwner->GetWorld();

	//@Spawn
	UParticleSystemComponent* Ret = UGameplayStatics::SpawnEmitterAttached
	(
		PT, CharactorOwner->GetMesh(), AttachName,
		FVector(0.0f), FRotator(0.0f), FVector(1.0f),
		AttachLocation
	);

	return Ret;
}

UParticleSystemComponent * UCMeshParticleComp::SpawnParticleAtMesh(UParticleSystem * const PT, const EAttachPointType Point, const EAttachPointRelative Rel, const EAttachLocation::Type AttachLocation, FTransform Transform)
{
	check(PT);

	FName AttachName;
	const bool bResult = CheckAttackName(AttachName, Point, Rel);

	UWorld* const World = CharactorOwner->GetWorld();

	//@Spawn
	UParticleSystemComponent* Ret = UGameplayStatics::SpawnEmitterAttached
	(
		PT, CharactorOwner->GetMesh(), AttachName,
		Transform.GetLocation(),
		FRotator(Transform.GetRotation()),
		Transform.GetScale3D(),
		AttachLocation
	);

	return Ret;
}

bool UCMeshParticleComp::SetLocationParticleCompAtMesh(UParticleSystemComponent * const PTComp, const EAttachPointType Point, const EAttachPointRelative Rel)
{
	check(PTComp);

	FName AttachName = "";
	const bool bResult = CheckAttackName(AttachName, Point, Rel);

	FVector Location = CharactorOwner->GetMesh()->GetSocketLocation(AttachName);
	PTComp->SetWorldLocation(Location);

	return bResult;
}

void UCMeshParticleComp::AttachParticleCompAtMesh(UParticleSystemComponent * const PTComp, const EAttachPointType Point, const EAttachPointRelative Rel)
{
	check(PTComp);

	FName AttachName = "";
	const bool bResult = CheckAttackName(AttachName, Point, Rel);

	bool bAttach = PTComp->AttachToComponent
	(
		CharactorOwner->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true),
		AttachName
	);

	PTComp->SetupAttachment(CharactorOwner->GetMesh(), AttachName);
}

bool UCMeshParticleComp::CheckAttackName(FName & OutName, const EAttachPointType Point, const EAttachPointRelative Rel)
{
	FString Temp = "";
	if (Rel == EAttachPointRelative::RELATIVE)
	{
		Temp.Append(RelativeTypeName.ToString());
	}

	switch (Point)
	{
		case EAttachPointType::ROOT:
			Temp.Empty();
			Temp.Append(AttachRootName.ToString());
			break;

		case EAttachPointType::HEAD:
			Temp.Append(AttachHeadName.ToString());
			break;

		case EAttachPointType::BODY:
			Temp.Append(AttachBodyName.ToString());
			break;

		case EAttachPointType::LHAND:
			Temp.Append(AttachLeftHandName.ToString());
			break;

		case EAttachPointType::RHAND:
			Temp.Append(AttachRightHandName.ToString());
			break;

		default:
			Temp.Append(AttachRootName.ToString());
			return false;
			break;
	}

	//@Result
	OutName = FName(*Temp);
	return true;
}

