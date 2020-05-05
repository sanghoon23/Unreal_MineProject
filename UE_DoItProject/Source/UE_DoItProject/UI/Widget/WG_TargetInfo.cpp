#include "WG_TargetInfo.h"
#include "CLog.h"
#include "UMG.h"
#include "Math/Color.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

UWG_TargetInfo::UWG_TargetInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWG_TargetInfo::NativeConstruct()
{
	Super::NativeConstruct();

	//@Get Player
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn != nullptr)
	{
		Player = Cast<ACPlayer>(PlayerPawn);
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Warning, L"UWG_TargetInfo Player NULL!!");
		}
	}
}

void UWG_TargetInfo::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	AActor* Target = Player->GetFindAttackTarget();
	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"UWG_TargetInfo NativeTick Target NULL!!");

		//## �ڱ� �ڽſ��� Visible �� ���� ����. ##
		//## TargetSystem ���� �ϰ� ����
		InitTargetInfo();

		return;
	}

	//@Get
	FVector PlayerLocation = Player->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	//@IC_Monster
	IIC_Monster* InterfaceMonster = Cast<IIC_Monster>(Target);
	if (InterfaceMonster != nullptr)
	{
		//## �ڱ� �ڽſ��� Visible �� ���� ����. ##
		//## TargetSystem ���� �ϰ� ����

		//@Get MonsterInfo
		TargetInfo = InterfaceMonster->GetMonsterInfo();
		TargetInfo.Distance = Player->GetDistanceTo(Target);
	}

	//@IC_HitComp - �����̻�, ConditionData �� �������� ����,
	IIC_Charactor* InterfaceCharactor = Cast<IIC_Charactor>(Target);
	if (InterfaceCharactor != nullptr)
	{
		IIC_HitComp* InterfaceHitComp = InterfaceCharactor->GetIHitComp();
		if (InterfaceHitComp != nullptr)
		{
			//@��ü������ GetConditionDatas �Լ����� Empty �� ����.
			InterfaceHitComp->GetConditionDatasAfterEmpty
			(
				&(TargetInfo.InfoConditionDataArray),
				ConditionUITextureNumber
			);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UWG_TargetInfo::InitTargetInfo()
{
	TargetInfo.Name = "";
	TargetInfo.HP = 0.0f;
	TargetInfo.Distance = 0.0f;
	TargetInfo.InfoConditionDataArray.Empty();
}

UTexture2D * UWG_TargetInfo::GetInfoConditionTextureUI(int ArrayNumber)
{
	int ArraySize = TargetInfo.InfoConditionDataArray.Num();
	if (ArrayNumber >= ArraySize || ArraySize < 0)
		return nullptr;

	return TargetInfo.InfoConditionDataArray[ArrayNumber]->TextureUI;
}

FLinearColor UWG_TargetInfo::GetInfoConditionDataLinearColor(int ArrayNumber)
{
	int ArraySize = TargetInfo.InfoConditionDataArray.Num();
	if (ArrayNumber >= ArraySize || ArraySize < 0)
		return FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 0.0f)); //@Alpha 0

	if (TargetInfo.InfoConditionDataArray[ArrayNumber] == nullptr)
		return FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 0.0f)); //@Alpha 0

	return TargetInfo.InfoConditionDataArray[ArrayNumber]->ColorAndOpacity;
}

UCBaseConditionType * UWG_TargetInfo::GetInfoMonsterConditionData(int ArrayNumber)
{
	int ArraySize = TargetInfo.InfoConditionDataArray.Num();
	if (ArrayNumber >= ArraySize || ArraySize < 0)
		return nullptr;

	return TargetInfo.InfoConditionDataArray[ArrayNumber];
}

TArray<class UCBaseConditionType*> UWG_TargetInfo::GetInfoMonsterConditionDataArray()
{
	return TargetInfo.InfoConditionDataArray;
}

void UWG_TargetInfo::WigetVisible()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWG_TargetInfo::WigetUnVisible()
{
	SetVisibility(ESlateVisibility::Hidden);
}
