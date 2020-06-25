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

	if (bTickTimer == true)
	{
		CurrentData.Timer -= InDeltaTime;

		if (CurrentData.Timer < 1.5f)
		{
			CurrentData.ColorOpacity -= InDeltaTime;
			FMath::Clamp(CurrentData.ColorOpacity, 0.0f, 1.0f);
		}

		if (CurrentData.Timer < 0.0f)
		{
			WigetUnVisible();
		}
	}
}

void UWG_TextNotify::WigetVisible()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	bTickTimer = true;
}

void UWG_TextNotify::WigetUnVisible()
{
	SetVisibility(ESlateVisibility::Hidden);
	bTickTimer = false;

	InitUITextData(); //@초기화
}

FText UWG_TextNotify::GetCurrentText()
{
	return FText::FromString(CurrentData.CurrentText);
}

void UWG_TextNotify::CallingUITextNotify(const FString& InputText, float fTime)
{
	CurrentData.CurrentText = InputText;
	CurrentData.ColorOpacity = 1.0f;

	(fTime == 0.0f)
		? CurrentData.Timer = VisibleTime
		: CurrentData.Timer = fTime;

	WigetVisible(); //@활성화
}

void UWG_TextNotify::InitUITextData()
{
	CurrentData.Timer = 0.0f;
	CurrentData.ColorOpacity = 1.0f;
	CurrentData.CurrentText = "";
}

