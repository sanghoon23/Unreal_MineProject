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

	// 1. Montage �� �ִٸ�, Montage ���� Attach
	if (AttachMontage != nullptr)
	{
		OnEquipedCharacter->ActorAnimMonPlay(AttachMontage, 1.5f, true);
	}
	// 2. Montage �� ���ٸ�, Detach
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

	// 1. Montage �� �ִٸ�, Montage ���� Detach
	if (DetachMontage != nullptr)
	{
		OnEquipedCharacter->ActorAnimMonPlay(DetachMontage, 1.5f, true);
	}
	// 2. Montage �� ���ٸ�, Detach
	else if (DetachMontage == nullptr)
	{
		MeshDetach();

		// Delegate ��ߵɵ�. -> �Ű������� functional �ޱ�.
		OnEquipedCharacter->GetIEquipComp()->WeaponRelease();

		// Equiping == false
		OnEquipedCharacter->GetIEquipComp()->SetEquiping(false);
	}
}

