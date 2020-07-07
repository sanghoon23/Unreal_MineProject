#include "WG_TargetInfo.h"
#include "CLog.h"
#include "UMG.h"
#include "Math/Color.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_WidgetInfo.h"
#include "Charactor/Player/CPlayer.h"

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

		//## 자기 자신에서 Visible 이 되지 않음. ##
		//## TargetSystem 에서 하고 있음
		InitTargetInfo();

		return;
	}

	FVector PlayerLocation = Player->GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();

	//@IC_Monster - TargetInfo 가져오기
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Target);
	if (I_Monster != nullptr)
	{
		//## 자기 자신에서 Visible 이 되지 않음. ##
		//## TargetSystem 에서 하고 있음

		//@Get MonsterInfo
		InsertTargetInfo(I_Monster->GetMonsterInfo());
		TargetInfo.Distance = Player->GetDistanceTo(Target);
	}

	//@Get Condition Data
	IIC_WidgetInfo* I_Widget = Cast<IIC_WidgetInfo>(Target);
	if (I_Widget != nullptr)
	{
		//@자체적으로 GetConditionDatas 함수에서 Empty 로 갱신.
		TargetInfo.InfoConditionDataArray.Empty();
		I_Widget->GetViewConditionStateForUI(&(TargetInfo.InfoConditionDataArray));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UWG_TargetInfo::InitTargetInfo()
{
	TargetInfo.Name = "";
	TargetInfo.MaxHP = 0.0f;
	TargetInfo.CurrentHP = 0.0f;
	TargetInfo.Distance = 0.0f;
	TargetInfo.InfoConditionDataArray.Empty();
}

void UWG_TargetInfo::InsertTargetInfo(const FMonsterInfo & Insert)
{
	TargetInfo.Name = Insert.Name;
	TargetInfo.MaxHP = Insert.MaxHP;
	TargetInfo.CurrentHP = Insert.CurrentHP;
	//TargetInfo.Distance = 0.0f;
}

float UWG_TargetInfo::GetInfoMonsterPercentHP() const
{
	return TargetInfo.CurrentHP / TargetInfo.MaxHP;
}

UTexture2D * UWG_TargetInfo::GetInfoConditionTextureUI(int ArrayNumber)
{
	int ArraySize = TargetInfo.InfoConditionDataArray.Num();
	if (ArrayNumber >= ArraySize || ArraySize < 0)
		return nullptr;

	class UTexture2D* RetTexture 
		= TargetInfo.InfoConditionDataArray[ArrayNumber].TextureUI;
	if (RetTexture == nullptr)
		return nullptr;

	return RetTexture;
}

FLinearColor UWG_TargetInfo::GetInfoConditionDataLinearColor(int ArrayNumber)
{
	int ArraySize = TargetInfo.InfoConditionDataArray.Num();
	if (ArrayNumber >= ArraySize || ArraySize < 0)
		return FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 0.0f)); //@Alpha 0

	return TargetInfo.InfoConditionDataArray[ArrayNumber].ColorAndOpacity;
}

FSlateColor UWG_TargetInfo::GetInfoConditionDataTintColor(int ArrayNumber)
{
	int ArraySize = TargetInfo.InfoConditionDataArray.Num();
	if (ArrayNumber >= ArraySize || ArraySize < 0)
		return FSlateColor(FLinearColor(FVector4(1.0f, 0.0f, 1.0f, 0.0f)));

	return TargetInfo.InfoConditionDataArray[ArrayNumber].TintSlateColor;
}

void UWG_TargetInfo::WigetVisible()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWG_TargetInfo::WigetUnVisible()
{
	SetVisibility(ESlateVisibility::Hidden);
}
