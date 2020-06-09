#include "CHM_BasicSword.h"
#include "Global.h"

ACHM_BasicSword::ACHM_BasicSword()
{
	// @FALSE
	PrimaryActorTick.bCanEverTick = false;

	// Super
	HandItemType = UHandItemType::SWORD;
	ItemAttachName = L"index_01_r";
	ItemEquipName = L"index_01_r";

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
		FString path = L"SkeletalMesh'/Game/_Mine/Mesh/HM_Basic/Axes/Blade_Hatchet02/SK_Blade_Hatchet02.SK_Blade_Hatchet02'";
		ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(*path);
		if (mesh.Succeeded())
			SkeletalMesh->SetSkeletalMesh(mesh.Object);
	}

	// @Attach, Detach Monatge .. NONE
	{
		//// AttachMontage
		//FString path = L"";
		//ConstructorHelpers::FObjectFinder<UAnimMontage> attachMontage(*path);
		//if (attachMontage.Succeeded())
		//	AttachMontage = attachMontage.Object;

		//// DetachMontage
		//path = L"AnimMontage'/Game/_Mine/Montages/Player/MCommon/WeaponSwap_Cast_Montage.WeaponSwap_Cast_Montage'";
		//ConstructorHelpers::FObjectFinder<UAnimMontage> detachMontage(*path);
		//if (detachMontage.Succeeded())
		//	DetachMontage = detachMontage.Object;
	}
}

void ACHM_BasicSword::BeginPlay()
{
	Super::BeginPlay();
}

void ACHM_BasicSword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
