#include "CHM_ShamanMagicStick.h"
#include "Global.h"

ACHM_ShamanMagicStick::ACHM_ShamanMagicStick()
{
	// @FALSE
	PrimaryActorTick.bCanEverTick = false;

	// Super
	HandItemType = UHandItemType::STICK;
	ItemAttachName = L"hand_r";
	ItemEquipName = L"hand_r";

	// @Create Component
	{
		Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
		RootComponent = Capsule;

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
	}

	// @Mesh
	{
		// Before Code
		FString path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Shaman/Blunt_Bell/SK_Blunt_Bell.SK_Blunt_Bell'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(*path);
		if (mesh.Succeeded())
			SkeletalMesh->SetSkeletalMesh(mesh.Object);
	}

	// @Attach, Detach Monatge .. NONE
}

void ACHM_ShamanMagicStick::BeginPlay()
{
	Super::BeginPlay();
}

void ACHM_ShamanMagicStick::SetMeshVisible(bool bSet)
{
	SkeletalMesh->SetVisibility(bSet);
}

void ACHM_ShamanMagicStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

