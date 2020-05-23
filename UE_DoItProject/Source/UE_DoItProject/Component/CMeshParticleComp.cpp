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

//void UCMeshParticleComp::AddParticleCompAtContainer(UParticleSystemComponent * const PTComp, const AttachPointType Point)
//{
//	check(PTComp);
//
//	FName AttachName;
//	const bool bResult = CheckAttackName(AttachName, Point);
//
//	FMeshParticle Insert;
//	Insert.TypeName = AttachName;
//	Insert.ParticleComp = PTComp;
//
//	ApplyParticles.Add(Insert);
//}

void UCMeshParticleComp::SpawnParticleAtMesh(UParticleSystem * const PT, const AttachPointType Point, const EAttachLocation::Type AttachLocation)
{
	check(PT);

	FName AttachName;
	const bool bResult = CheckAttackName(AttachName, Point);

	UWorld* const World = CharactorOwner->GetWorld();

	//@Spawn
	UGameplayStatics::SpawnEmitterAttached
	(
		PT, CharactorOwner->GetMesh(), AttachName,
		FVector(0.0f), FRotator(0.0f), FVector(1.0f),
		AttachLocation
	);

}

bool UCMeshParticleComp::SetLocationParticleCompAtMesh(UParticleSystemComponent * const PTComp, const AttachPointType Point)
{
	check(PTComp);

	FName AttachName = "";
	const bool bResult = CheckAttackName(AttachName, Point);

	FVector Location = CharactorOwner->GetMesh()->GetSocketLocation(AttachName);
	PTComp->SetWorldLocation(Location);

	return bResult;
}

bool UCMeshParticleComp::CheckAttackName(FName & OutName, const AttachPointType Point)
{
	switch (Point)
	{
		case AttachPointType::ROOT:
			OutName = AttachRootName;
			return true;
			break;

		case AttachPointType::HEAD:
			OutName = AttachHeadName;
			return true;
			break;

		case AttachPointType::BODY:
			OutName = AttachBodyName;
			return true;
			break;

		case AttachPointType::LHAND:
			OutName = AttachLeftHandName;
			return true;
			break;

		case AttachPointType::RHAND:
			OutName = AttachRightHandName;
			return true;
			break;

		default:
			OutName = AttachRootName;
			return false;
			break;
	}
}

