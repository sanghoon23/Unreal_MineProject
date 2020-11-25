#include "CPL_EquipComp.h"
#include "Global.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_StateManager.h"
#include "Item/Base/CItem_Hand.h"
#include "Item/CPL_Sword.h"

UCPL_EquipComp::UCPL_EquipComp()
{
	// @FALSE
	PrimaryComponentTick.bCanEverTick = true;
}


void UCPL_EquipComp::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform = FTransform::Identity;
	FActorSpawnParameters params;
	params.Owner = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	#pragma region Create DisplayItem List
	// Spawn Sword
	{
		ACPL_Sword* Sword = GetWorld()->SpawnActor<ACPL_Sword>(ACPL_Sword::StaticClass(), transform, params);

		Sword->AttachToComponent
		(
			Cast<ACharacter>(GetOwner())->GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
			, *(Sword->GetItemAttachName())
		);
		Sword->SetActorRelativeRotation(FRotator(180.f, 17.f, -1.f));
		Sword->SetActorRelativeLocation(FVector(-2.3f, 1.5f, -1.16f));

		// @Add - �켱 DisplayList ���
		DisplayList.Add(Sword);
		// ItemMapAdd(Sword->GetItemType(), Sword);
	}
	#pragma endregion

	// Set Delegate "On Hit" - 

	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IIC_Player* IC_Player = Cast<IIC_Player>(GetOwner());
	check(IC_Player);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bEquiping = false;
	});

	IIC_StateManager* IC_State = IC_Player->GetIStateManager();
	check(IC_State);
	IC_State->OnSwordState.AddLambda([&]()
	{
		Cast<ACItem_Hand>(DisplayList[0])->OnEquip();
		DisplayList[0]->GetStaticMeshComp()->SetVisibility(true);
	});

	IC_State->UnSwordState.AddLambda([&]()
	{
		Cast<ACItem_Hand>(DisplayList[0])->UnEquip();
		DisplayList[0]->GetStaticMeshComp()->SetVisibility(false);
	});


#pragma region Set Delegate - Charactor	
	//@OnDeath ȣ�� ��, ItemList UnVisible - Death �� Respawn �� �� ����.
	IC_Charactor->OnDeathDelegate.AddLambda([&]()
	{
		for (ACItem_Hand* ItemActor : DisplayList)
		{
			ItemActor->GetStaticMeshComp()->SetVisibility(false);
		}
	});

	IC_Charactor->OnInitDelegate.AddLambda([&]()
	{
		bEquiping = false;
		bArmed = false;
		CurrentWeaponNum = -1;
	});

#pragma endregion
}

void UCPL_EquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - ������ ����
// @CurrentWeaponNum = -1, �� ���ÿ�,
// @CurrentWeapon(ACDisplayItem*) == NULL
// @bArmed = false
void UCPL_EquipComp::WeaponRelease()
{
	CurrentWeaponNum = -1;
	CurrentWeapon = nullptr;

	bArmed = false;
}

// - ������ Swap
// @AscendingOrder - EquipmentComp �� �����ϴ� 'Weapon Array' ���� ������ ����.
// ��,	true - ��������
//		false - ��������
void UCPL_EquipComp::WeaponNextSwap(bool AscendingOrder)
{
	TArray<ACItem_Hand*>* HandWeaponArray = DisplayMap.Find(UItemType::HAND);
	check(HandWeaponArray);

	// Calc
	AscendingOrder 
		? ++CurrentWeaponNum 
		: --CurrentWeaponNum;

	if (CurrentWeaponNum > HandWeaponArray->Num() - 1
		|| CurrentWeaponNum == -1)
	{
		// WeaponRelase �ع�����, Notify ����������
		// ���Ⱑ Nullptr �� �ǹ����� Detach �� ����.
		// WeaponRelease();
		Cast<ACItem_Hand>(CurrentWeapon)->UnEquip();

		return;
	}
	if (CurrentWeaponNum < -1)
	{
		CurrentWeaponNum = HandWeaponArray->Num() - 1;
	}

	// Setting
	{
		// ���� socket ���� ���� MeshDetach - GetItemAttachName.
		ACharacter* SettingCharacter = Cast<ACharacter>(GetOwner());
		check(SettingCharacter);
		if (CurrentWeapon != nullptr)
		{
			CurrentWeapon->MeshDetach();
		}

		// (++) �� ���� ���� ����.
		CurrentWeapon = (*HandWeaponArray)[CurrentWeaponNum];
		check(CurrentWeapon);

		// Item Equip
		Cast<ACItem_Hand>(CurrentWeapon)->OnEquip();

		// UnArmed
		bArmed = true;
	}
}


/* DisplayList Size üũ���־����. */
ACItem_Hand * UCPL_EquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum < 0
		&& WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

// DisplayMap - TMap<UItemType, TArray<class ACDisplayItem*>>
// @Type - ItemHand �� ���� �Ǿ��ִ� Type ���� Map Insert
// @CItem - Map �� ������ ������(ACDisplayItem)
void UCPL_EquipComp::ItemMapAdd(UItemType Type, ACItem_Hand * CItem)
{
	IfNullRet(CItem);

	TArray<ACItem_Hand*>* ArrayValue = DisplayMap.Find(Type);
	if (ArrayValue == nullptr) // TArray ����.
	{
		TArray<ACItem_Hand*> insertArrayValue;
		insertArrayValue.Add(CItem);
		DisplayMap.Add(Type, insertArrayValue);
	}
	else // TArray �� ����.
	{
		ArrayValue->Add(CItem);
	}
}

// DisplayMap - TMap<UItemType, TArray<class ACItem_Hand*>>
// @Type - ItemHand �� ���� �Ǿ��ִ� Type , Ex)Hand, Static...��
// @FindItemNum - TMap Type �� �ش��ϴ� Array ACItem_Hand �� Number
// @Return - �ֻ��� �θ� ��ȯ��, Ex) ACDisplay -> ItemHand -> Sword..
ACItem_Hand * UCPL_EquipComp::ItemMapFind(UItemType Type, int FindItemNum)
{
	TArray<ACItem_Hand*>* Item = DisplayMap.Find(Type);
	IfNullRetResult(Item, nullptr);

	return (*Item)[FindItemNum];
}

