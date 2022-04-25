#include "CHM_ShamanEquipComp.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"

#include "Item/CHM_ShamanMagicStick.h"

UCHM_ShamanEquipComp::UCHM_ShamanEquipComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCHM_ShamanEquipComp::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Create Display List

	FTransform transform = FTransform::Identity;
	FActorSpawnParameters params;
	params.Owner = GetOwner();

	MagicStick = GetWorld()->SpawnActor<ACHM_ShamanMagicStick>(ACHM_ShamanMagicStick::StaticClass(), params);
	check(MagicStick);

	// Spawn Sword
	{
		MagicStick->AttachToComponent
		(
			Cast<ACharacter>(GetOwner())->GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
			, *(MagicStick->GetItemAttachName())
		);
		MagicStick->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		MagicStick->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		/*
		#1125_
		Sequence Stage_1_MonsterSpawnSeq 에서 EquipComponent
		Actviate, DeActivate 이벤트 활성화 비활성화로
		무기를 끄고 키게끔 구현.
		*/
		MagicStick->SetMeshVisible(true);

		DisplayList.Add(MagicStick);
	}

	#pragma endregion

	#pragma region Set Delegate - Charactor	
	//@OnDeath 호출 시, Sword 제거 
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(GetOwner());
	if (I_Charactor != nullptr)
	{
		I_Charactor->OnDeathDelegate.AddLambda([&]()
		{
			MagicStick->Destroy();
		});
	}

	#pragma endregion

}


// Called every frame
void UCHM_ShamanEquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACItem_Hand * UCHM_ShamanEquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

void UCHM_ShamanEquipComp::OnMeshVisible()
{
	MagicStick->SetMeshVisible(true);
}

void UCHM_ShamanEquipComp::OffMeshVisible()
{
	MagicStick->SetMeshVisible(false);
}
