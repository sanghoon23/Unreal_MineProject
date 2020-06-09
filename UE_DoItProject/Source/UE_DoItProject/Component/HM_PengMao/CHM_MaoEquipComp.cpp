#include "CHM_MaoEquipComp.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"

#include "Item/CHM_MaoBlade.h"


UCHM_MaoEquipComp::UCHM_MaoEquipComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCHM_MaoEquipComp::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Create Display List

	FTransform transform = FTransform::Identity;
	FActorSpawnParameters params;
	params.Owner = GetOwner();

	MaoBlade = GetWorld()->SpawnActor<ACHM_MaoBlade>(ACHM_MaoBlade::StaticClass(), params);
	check(MaoBlade);

	// Spawn Sword
	{
		MaoBlade->AttachToComponent
		(
			Cast<ACharacter>(GetOwner())->GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
			, *(MaoBlade->GetItemAttachName())
		);
		MaoBlade->SetActorRelativeRotation(FRotator(3.0f, 90.0f, 0.0f));
		MaoBlade->SetActorRelativeLocation(FVector(10.0f, 10.0f, 0.0f));

		//MaoBlade->SetActorRelativeRotation(FRotator(18.37f, 22.7f, -58.6f));
		//MaoBlade->SetActorRelativeLocation(FVector(28.0f, 0.0f, 140.0f));

		DisplayList.Add(MaoBlade);
	}

	#pragma endregion

	#pragma region Set Delegate - Charactor	
	//@OnDeath 호출 시, Sword 제거 
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(GetOwner());
	if (I_Charactor != nullptr)
	{
		I_Charactor->OnDeathDelegate.AddLambda([&]()
		{
			MaoBlade->Destroy();
		});
	}

	#pragma endregion
}

void UCHM_MaoEquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACItem_Hand * UCHM_MaoEquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}
