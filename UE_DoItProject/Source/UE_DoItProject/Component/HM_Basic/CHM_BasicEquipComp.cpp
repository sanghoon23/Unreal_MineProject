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

	Sword = GetWorld()->SpawnActor<ACHM_BasicSword>(ACHM_BasicSword::StaticClass(), params);
	check(Sword);

	// Spawn Sword
	{
		Sword->AttachToComponent
		(
			Cast<ACharacter>(GetOwner())->GetMesh() //@Charactor Mesh
			, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true)
			, *(Sword->GetItemAttachName())
		);
		Sword->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Sword->SetActorRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		/*
		#1125_
		Sequence Stage_1_MonsterSpawnSeq 에서 EquipComponent
		Actviate, DeActivate 이벤트 활성화 비활성화로
		무기를 끄고 키게끔 구현.
		*/
		Sword->SetMeshVisible(false);

		DisplayList.Add(Sword);
	}

	#pragma endregion

	#pragma region Set Delegate - Charactor	
	//@OnDeath 호출 시, Sword 제거 
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(GetOwner());
	if (I_Charactor != nullptr)
	{
		I_Charactor->OnDeathDelegate.AddLambda([&]()
		{
			Sword->Destroy();
		});
	}

	#pragma endregion

	//@#1125_ Cinematic Intro 에서 무기가 보임.....
	OnComponentActivated.AddDynamic(this, &UCHM_BasicEquipComp::CompActivated);
	OnComponentDeactivated.AddDynamic(this, &UCHM_BasicEquipComp::CompDeActivated);
}


void UCHM_BasicEquipComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

ACItem_Hand * UCHM_BasicEquipComp::GetDisplayItem(int WeaponArrayNum)
{
	if (WeaponArrayNum > DisplayList.Num() - 1)
		return nullptr;

	return DisplayList[WeaponArrayNum];
}

void UCHM_BasicEquipComp::OnMeshVisible()
{
	Sword->SetMeshVisible(true);
}

void UCHM_BasicEquipComp::OffMeshVisible()
{
	Sword->SetMeshVisible(false);
}

void UCHM_BasicEquipComp::CompActivated(UActorComponent * Component, bool bReset)
{
	CLog::Print(L"EquipComp Activated!!");
	Sword->SetMeshVisible(true);
}

void UCHM_BasicEquipComp::CompDeActivated(UActorComponent * Component)
{
	CLog::Print(L"EquipComp 'De' Activated!!");
	Sword->SetMeshVisible(false);
}
