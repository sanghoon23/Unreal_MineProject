#include "CHM_BasicEquipComp.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"

#include "Item/CHM_BasicSword.h"

UCHM_BasicEquipComp::UCHM_BasicEquipComp()
{
	// @FALSE
	PrimaryComponentTick.bCanEverTick = false;
}


void UCHM_BasicEquipComp::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Create Display List

	FTransform transform = FTransform::Identity;
	FActorSpawnParameters params;
	params.Owner = GetOwner();

	// Spawn Sword
	{
		ACHM_BasicSword* Sword = GetWorld()->SpawnActor<ACHM_BasicSword>(ACHM_BasicSword::StaticClass(), params);
		check(Sword);

		Sword->AttachToComponent
		(
			Cast<ACharacter>(GetOwner())->GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
			, *(Sword->GetItemAttachName())
		);
		Sword->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Sword->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		DisplayList.Add(Sword);
	}

	#pragma endregion
}


void UCHM_BasicEquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACDisplayItem * UCHM_BasicEquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

