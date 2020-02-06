#include "CPL_Sword.h"
#include "Global.h"

ACPL_Sword::ACPL_Sword()
{
	PrimaryActorTick.bCanEverTick = true;

	// Super
	HandItemType = UHandItemType::SWORD;
	ItemAttachName = L"middle_02_r";
	ItemEquipName = L"middle_02_r";

	// Create
	{
		// Before Code
		FString path = L"StaticMesh'/Game/_Mine/Mesh/Item/Player_Sword.Player_Sword'";
		ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(*path);
		if (mesh.Succeeded())
			StaticMesh->SetStaticMesh(mesh.Object);
	}

	// Montage
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

	// Setting
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Capsule->SetGenerateOverlapEvents(true);
		// Capsule->SetCollisionProfileName("BlockAll");
		Capsule->SetVisibility(true);
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
