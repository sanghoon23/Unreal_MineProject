#include "CPlayerEquipComp.h"
#include "Global.h"

#include "Charactor/Player/CPlayer.h"
#include "Item/Base/CItem_Hand.h"
#include "Item/CPL_Sword.h"

UCPlayerEquipComp::UCPlayerEquipComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCPlayerEquipComp::BeginPlay()
{
	Super::BeginPlay();

	FTransform transform = FTransform::Identity;
	FActorSpawnParameters params;
	params.Owner = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	#pragma region Create DisplayItem List
	// Spawn Sword
	{
		ACPL_Sword* Sword 
			= GetWorld()->SpawnActor<ACPL_Sword>(ACPL_Sword::StaticClass(), transform, params);

		Sword->AttachToComponent
		(
			Cast<ACharacter>(GetOwner())->GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
			, *(Sword->GetItemAttachName())
		);
		Sword->SetActorRelativeRotation(FRotator(180.f, 17.f, -1.f));
		Sword->SetActorRelativeLocation(FVector(2.75f, 2.5f, -1.16f));

		// @Add - 우선 DisplayList 사용
		DisplayList.Add(Sword);
		// ItemMapAdd(Sword->GetItemType(), Sword);
	}
	#pragma endregion

	// Set Delegate "On Hit" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnHitResetState.AddLambda([&](AActor*)
	{
		bEquiping = false;
	});
}

void UCPlayerEquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - 아이템 해제
// @CurrentWeaponNum = -1, 과 동시에,
// @CurrentWeapon(ACDisplayItem*) == NULL
// @bArmed = false
void UCPlayerEquipComp::WeaponRelease()
{
	CurrentWeaponNum = -1;
	CurrentWeapon = nullptr;

	bArmed = false;
}

// - 아이템 Swap
// @AscendingOrder - EquipmentComp 에 존재하는 'Weapon Array' 에서 스왑할 방향.
// 즉,	true - 오름차순
//		false - 내림차순
void UCPlayerEquipComp::WeaponNextSwap(bool AscendingOrder)
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

	CLog::Print(L"EquipComp Swap In!!");
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


void UCPlayerEquipComp::ItemMapAdd(UItemType Type, ACDisplayItem * CItem)
{
	IfNullRet(CItem);

	TArray<ACDisplayItem*>* ArrayValue = DisplayMap.Find(Type);
	if (ArrayValue == nullptr) // TArray 생성.
	{
		TArray<ACDisplayItem*> insertArrayValue;
		insertArrayValue.Add(CItem);
		DisplayMap.Add(Type, insertArrayValue);
	}
	else // TArray 에 삽입.
	{
		ArrayValue->Add(CItem);
	}
}

ACDisplayItem * UCPlayerEquipComp::ItemMapFind(UItemType Type, int FindItemNum)
{
	TArray<ACDisplayItem*>* Item = DisplayMap.Find(Type);
	IfNullRetResult(Item, nullptr);

	return (*Item)[FindItemNum];
}

