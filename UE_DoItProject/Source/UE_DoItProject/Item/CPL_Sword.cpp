#include "CPL_Sword.h"
#include "Global.h"

ACPL_Sword::ACPL_Sword()
{
	// @FALSE
	PrimaryActorTick.bCanEverTick = false;

	// Super
	HandItemType = UHandItemType::SWORD;
	ItemAttachName = L"hand_r";
	ItemEquipName = L"hand_r";

	// @Create Component
	{
		Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
		RootComponent = Capsule;

		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
		StaticMesh->SetupAttachment(Capsule);
	}

	// @Setting
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Capsule->SetGenerateOverlapEvents(true);
		// Capsule->SetCollisionProfileName("BlockAll");
		Capsule->SetVisibility(true);

		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetEnableGravity(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	// @Mesh
	{
		// Before Code
		FString path = L"StaticMesh'/Game/_Mine/Mesh/Item/Player_Sword.Player_Sword'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path);
		if (mesh.Succeeded())
			StaticMesh->SetStaticMesh(mesh.Object);
	}

	// @Montage
	{
		// AttachMontage
		FString path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/Sword_Equip_Montage.Sword_Equip_Montage'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> attachMontage(*path);
		if (attachMontage.Succeeded())
		AttachMontage = attachMontage.Object;

		//// DetachMontage
		//path = L"AnimMontage'/Game/_Mine/Montages/Player/MCommon/WeaponSwap_Cast_Montage.WeaponSwap_Cast_Montage'";
		//ConstructorHelpers::FObjectFinder<UAnimMontage> detachMontage(*path);
		//if (detachMontage.Succeeded())
		//	DetachMontage = detachMontage.Object;
	}
}

void ACPL_Sword::BeginPlay()
{
	Super::BeginPlay();
}

void ACPL_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
