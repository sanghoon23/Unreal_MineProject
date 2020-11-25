#include "CHM_MaoBlade.h"
#include "Global.h"

ACHM_MaoBlade::ACHM_MaoBlade()
{
	// @FALSE
	PrimaryActorTick.bCanEverTick = false;

	// Super
	HandItemType = UHandItemType::SWORD;
	ItemAttachName = L"index_01_r";
	ItemEquipName = L"index_01_r";

	// @Create Component
	{
		Scene = CreateDefaultSubobject<USceneComponent>("Scene");
		RootComponent = Scene;

		Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
		Capsule->SetupAttachment(Scene);

		SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
		SkeletalMesh->SetupAttachment(Capsule);
	}

	// @Setting
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Capsule->SetGenerateOverlapEvents(true);
		// Capsule->SetCollisionProfileName("BlockAll");
		Capsule->SetVisibility(true);

		SkeletalMesh->SetSimulatePhysics(false);
		SkeletalMesh->SetEnableGravity(false);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SkeletalMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 3.0f));
		SkeletalMesh->SetRelativeRotation(FRotator(18.0f, 0.0f, 14.0f));
		SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -45.0f));
	}

	// @Mesh
	{
		FString path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_PengMAo/Blunt/Blunt_Primative/SK_Blunt_Primative.SK_Blunt_Primative'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(*path);
		if (mesh.Succeeded())
			SkeletalMesh->SetSkeletalMesh(mesh.Object);
	}
}

void ACHM_MaoBlade::BeginPlay()
{
	Super::BeginPlay();
}

void ACHM_MaoBlade::SetMeshVisible(bool bSet)
{
	SkeletalMesh->SetVisibility(bSet);
}

void ACHM_MaoBlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
