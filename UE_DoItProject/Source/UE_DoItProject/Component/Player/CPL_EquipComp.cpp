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

		// @Add - 우선 DisplayList 사용
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
	//@OnDeath 호출 시, ItemList UnVisible - Death 후 Respawn 될 수 있음.
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

// - 아이템 해제
// @CurrentWeaponNum = -1, 과 동시에,
// @CurrentWeapon(ACDisplayItem*) == NULL
// @bArmed = false
void UCPL_EquipComp::WeaponRelease()
{
	CurrentWeaponNum = -1;
	CurrentWeapon = nullptr;

	bArmed = false;
}

// - 아이템 Swap
// @AscendingOrder - EquipmentComp 에 존재하는 'Weapon Array' 에서 스왑할 방향.
// 즉,	true - 오름차순
//		false - 내림차순
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
		// WeaponRelase 해버리면, Notify 가기전까지
		// 무기가 Nullptr 이 되버려서 Detach 를 못함.
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
		// 이전 socket 으로 무기 MeshDetach - GetItemAttachName.
		ACharacter* SettingCharacter = Cast<ACharacter>(GetOwner());
		check(SettingCharacter);
		if (CurrentWeapon != nullptr)
		{
			CurrentWeapon->MeshDetach();
		}

		// (++) 한 현재 무기 세팅.
		CurrentWeapon = (*HandWeaponArray)[CurrentWeaponNum];
		check(CurrentWeapon);

		// Item Equip
		Cast<ACItem_Hand>(CurrentWeapon)->OnEquip();

		// UnArmed
		bArmed = true;
	}
}


/* DisplayList Size 체크해주어야함. */
ACItem_Hand * UCPL_EquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum < 0
		&& WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

// DisplayMap - TMap<UItemType, TArray<class ACDisplayItem*>>
// @Type - ItemHand 에 정의 되어있는 Type 으로 Map Insert
// @CItem - Map 에 삽입할 아이템(ACDisplayItem)
void UCPL_EquipComp::ItemMapAdd(UItemType Type, ACItem_Hand * CItem)
{
	IfNullRet(CItem);

	TArray<ACItem_Hand*>* ArrayValue = DisplayMap.Find(Type);
	if (ArrayValue == nullptr) // TArray 생성.
	{
		TArray<ACItem_Hand*> insertArrayValue;
		insertArrayValue.Add(CItem);
		DisplayMap.Add(Type, insertArrayValue);
	}
	else // TArray 에 삽입.
	{
		ArrayValue->Add(CItem);
	}
}

// DisplayMap - TMap<UItemType, TArray<class ACItem_Hand*>>
// @Type - ItemHand 에 정의 되어있는 Type , Ex)Hand, Static...등
// @FindItemNum - TMap Type 에 해당하는 Array ACItem_Hand 의 Number
// @Return - 최상위 부모를 반환함, Ex) ACDisplay -> ItemHand -> Sword..
ACItem_Hand * UCPL_EquipComp::ItemMapFind(UItemType Type, int FindItemNum)
{
	TArray<ACItem_Hand*>* Item = DisplayMap.Find(Type);
	IfNullRetResult(Item, nullptr);

	return (*Item)[FindItemNum];
}

