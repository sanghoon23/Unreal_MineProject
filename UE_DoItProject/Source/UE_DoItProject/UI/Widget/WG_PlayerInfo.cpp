#include "WG_PlayerInfo.h"
#include "CLog.h"
#include "UMG.h"

#include "Interface/IC_Player.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"

UWG_PlayerInfo::UWG_PlayerInfo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
void UWG_PlayerInfo::NativeConstruct()
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

void UWG_PlayerInfo::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IIC_Player* I_Player = Cast<IIC_Player>(Player);
	check(I_Player);
	if (I_Player != nullptr)
	{
		PlayerInfo = I_Player->GetPlayerInfo();
	}

	//@IC_HitComp - �����̻�, ConditionData �� �������� ����,
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Player);
	if (I_Charactor != nullptr)
	{
		IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
		if (I_HitComp != nullptr)
		{
			//@��ü������ GetConditionDatas �Լ����� Empty �� ����.
			I_HitComp->GetConditionDatasAfterEmpty
			(
				&(PlayerInfo.InfoConditionDataArray),
				ConditionUITextureNumber
			);
		}
	}
}

void UWG_PlayerInfo::InitPlayerInfo()
{
	PlayerInfo.Name = "";
	PlayerInfo.CurrentHP = 0.0f;
	PlayerInfo.InfoConditionDataArray.Empty();
}

void UWG_PlayerInfo::WigetVisible()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWG_PlayerInfo::WigetUnVisible()
{
	SetVisibility(ESlateVisibility::Hidden);
}

