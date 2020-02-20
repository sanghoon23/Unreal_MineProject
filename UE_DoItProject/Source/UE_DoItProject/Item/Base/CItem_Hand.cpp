#include "CItem_Hand.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"

ACItem_Hand::ACItem_Hand()
{
	// Super
	ItemType = UItemType::HAND;
}

void ACItem_Hand::BeginPlay()
{
	Super::BeginPlay();
}

void ACItem_Hand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACItem_Hand::MeshAttach(FString StrAttachName)
{
	Super::MeshAttach(StrAttachName);
}

void ACItem_Hand::MeshDetach()
{
	Super::MeshDetach();
}

void ACItem_Hand::OnEquip()
{
	IIC_Charactor* OnEquipedCharacter = Cast<IIC_Charactor>(GetOwner());
	check(OnEquipedCharacter);

	OnEquipedCharacter->GetIEquipComp()->SetEquiping(true);

	// 1. Montage 가 있다면, Montage 에서 Attach
	if (AttachMontage != nullptr)
	{
		OnEquipedCharacter->ActorAnimMonPlay(AttachMontage, 1.5f, true);
	}
	// 2. Montage 가 없다면, Detach
	else if (AttachMontage == nullptr)
	{
		MeshAttach(Super::GetItemEquipName());

		OnEquipedCharacter->GetIEquipComp()->SetEquiping(false);
	}
}

void ACItem_Hand::UnEquip()
{
	IIC_Charactor* OnEquipedCharacter = Cast<IIC_Charactor>(GetOwner());
	check(OnEquipedCharacter);

	// Equiping == true
	OnEquipedCharacter->GetIEquipComp()->SetEquiping(true);

	// 1. Montage 가 있다면, Montage 에서 Detach
	if (DetachMontage != nullptr)
	{
		OnEquipedCharacter->ActorAnimMonPlay(DetachMontage, 1.5f, true);
	}
	// 2. Montage 가 없다면, Detach
	else if (DetachMontage == nullptr)
	{
		MeshDetach();

		// Delegate 써야될듯. -> 매개변수로 functional 받기.
		OnEquipedCharacter->GetIEquipComp()->WeaponRelease();

		// Equiping == false
		OnEquipedCharacter->GetIEquipComp()->SetEquiping(false);
	}
}

