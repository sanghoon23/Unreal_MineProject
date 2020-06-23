#include "WG_TextNotify.h"
#include "CLog.h"
#include "UMG.h"

#include "Charactor/Player/CPlayer.h"

UWG_TextNotify::UWG_TextNotify(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWG_TextNotify::NativeConstruct()
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

void UWG_TextNotify::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UWG_TextNotify::WigetVisible()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWG_TextNotify::WigetUnVisible()
{
	SetVisibility(ESlateVisibility::Hidden);
}

