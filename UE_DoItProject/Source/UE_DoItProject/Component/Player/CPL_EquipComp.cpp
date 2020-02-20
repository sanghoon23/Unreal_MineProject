#include "CPL_EquipComp.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
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
		Sword->SetActorRelativeLocation(FVector(2.75f, 2.5f, -1.16f));

		// @Add - �켱 DisplayList ���
		DisplayList.Add(Sword);
		// ItemMapAdd(Sword->GetItemType(), Sword);
	}
	#pragma endregion

	// Set Delegate "On Hit" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bEquiping = false;
	});

	IIC_StateManager* IC_State = IC_Charactor->GetIStateManager();
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
	TArray<ACDisplayItem*>* HandWeaponArray = DisplayMap.Find(UItemType::HAND);
	IfNullRetResult(HandWeaponArray, CLog::Print(L"WeaponSwap_HandWeaponArray NULL!!"));
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

	CLog::Print(L"EquipComp Swap In!!");
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
ACDisplayItem * UCPL_EquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum < 0
		&& WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

// DisplayMap - TMap<UItemType, TArray<class ACDisplayItem*>>
// @Type - ItemHand �� ���� �Ǿ��ִ� Type ���� Map Insert
// @CItem - Map �� ������ ������(ACDisplayItem)
void UCPL_EquipComp::ItemMapAdd(UItemType Type, ACDisplayItem * CItem)
{
	IfNullRet(CItem);

	TArray<ACDisplayItem*>* ArrayValue = DisplayMap.Find(Type);
	if (ArrayValue == nullptr) // TArray ����.
	{
		TArray<ACDisplayItem*> insertArrayValue;
		insertArrayValue.Add(CItem);
		DisplayMap.Add(Type, insertArrayValue);
	}
	else // TArray �� ����.
	{
		ArrayValue->Add(CItem);
	}
}

// DisplayMap - TMap<UItemType, TArray<class ACDisplayItem*>>
// @Type - ItemHand �� ���� �Ǿ��ִ� Type , Ex)Hand, Static...��
// @FindItemNum - TMap Type �� �ش��ϴ� Array ACDisplayItem �� Number
// @Return - �ֻ��� �θ� ��ȯ��, Ex) ACDisplay -> ItemHand -> Sword..
ACDisplayItem * UCPL_EquipComp::ItemMapFind(UItemType Type, int FindItemNum)
{
	TArray<ACDisplayItem*>* Item = DisplayMap.Find(Type);
	IfNullRetResult(Item, nullptr);

	return (*Item)[FindItemNum];
}

