#include "HUD_Main.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

#include "CLog.h"

AHUD_Main::AHUD_Main()
{
	FString strPath = L"";

	#pragma region LOAD UI SubOfClass
	//Player Info
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_PlayerInfo.BpCWG_PlayerInfo_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerInfoClass(*strPath);
	if (PlayerInfoClass.Succeeded())
	{
		PlayerInfoWidgetClass = PlayerInfoClass.Class;
	}

	//Target Info
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_TargetInfo.BpCWG_TargetInfo_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> TargetInfoClass(*strPath);
	if (TargetInfoClass.Succeeded())
	{
		TargetInfoWidgetClass = TargetInfoClass.Class;
	}

	//Skill Casting Bar
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_SkillCastingBar.BpCWG_SkillCastingBar_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> CastingClass(*strPath);
	if (CastingClass.Succeeded())
	{
		CastingWidgetClass = CastingClass.Class;
	}

	//Text Notify
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_TextNotify.BpCWG_TextNotify_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> TextNotifyClass(*strPath);
	if (TextNotifyClass.Succeeded())
	{
		TextNotifyWidgetClass = TextNotifyClass.Class;
	}

	//TODO :
	//Player Dead Menu
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_PlayerDeadMenu.BpCWG_PlayerDeadMenu_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> PlayerDeadMenuClass(*strPath);
	if (PlayerDeadMenuClass.Succeeded())
	{
		PlayerDeadMenuWidgetClass = PlayerDeadMenuClass.Class;
	}

	#pragma endregion
}

void AHUD_Main::DrawHUD()
{
	Super::DrawHUD();
}

void AHUD_Main::BeginPlay()
{
	Super::BeginPlay();

	//@PlayerInfo Widget
	if (PlayerInfoWidgetClass != nullptr)
	{
		PlayerInfoWidget = CreateWidget<UWG_PlayerInfo>(GetWorld(), PlayerInfoWidgetClass);
		if (PlayerInfoWidget)
		{
			PlayerInfoWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
			PlayerInfoWidget->AddToViewport();
		}
	}

	//@TargetInfo Widget
	if (TargetInfoWidgetClass != nullptr)
	{
		TargetInfoWidget = CreateWidget<UWG_TargetInfo>(GetWorld(), TargetInfoWidgetClass);
		if (TargetInfoWidget)
		{
			TargetInfoWidget->SetVisibility(ESlateVisibility::Hidden);
			TargetInfoWidget->AddToViewport();
		}
	}

	//@Casting Widget
	if (CastingWidgetClass != nullptr)
	{
		CastingWidget = CreateWidget<UWG_SkillCastingBar>(GetWorld(), CastingWidgetClass);
		if (CastingWidget)
		{
			CastingWidget->SetVisibility(ESlateVisibility::Hidden);
			CastingWidget->AddToViewport();
		}
	}

	//@Text Notify
	if (TextNotifyWidgetClass != nullptr)
	{
		TextNotifyWidget = CreateWidget<UWG_TextNotify>(GetWorld(), TextNotifyWidgetClass);
		if (TextNotifyWidget)
		{
			TextNotifyWidget->SetVisibility(ESlateVisibility::Hidden);
			TextNotifyWidget->AddToViewport();
		}
	}

	//@Player Dead Menu
	if (PlayerDeadMenuWidgetClass != nullptr)
	{
		PlayerDeadMenuWidget = CreateWidget<UWG_PlayerDeadMenu>(GetWorld(), PlayerDeadMenuWidgetClass);
		if (PlayerDeadMenuWidget)
		{
			PlayerDeadMenuWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerDeadMenuWidget->AddToViewport();
		}
	}
}

void AHUD_Main::VisibleUITextNotify(const FString& InputText, float fTime)
{
	TextNotifyWidget->CallingUITextNotify(InputText, fTime);
}

UWG_SkillCastingBar * AHUD_Main::GetWidgetSkillCastingBar()
{
	check(CastingWidget);
	return CastingWidget;
}

UWG_TargetInfo * AHUD_Main::GetWidgetTargetInfo()
{
	check(TargetInfoWidget);
	return TargetInfoWidget;
}

UWG_PlayerDeadMenu * AHUD_Main::GetWidgetPlayerDeadMenu()
{
	check(PlayerDeadMenuWidget);
	return PlayerDeadMenuWidget;
}

void AHUD_Main::SetInputWidget(TSubclassOf<UUserWidget> InsertWidget)
{
	if (InputWidget != nullptr)
	{
		InputWidget->RemoveFromViewport();
		InputWidget = nullptr;
	}

	if (InputWidget != nullptr)
	{
		InputWidget = CreateWidget<UUserWidget>(GetWorld(), InsertWidget);
		if (InputWidget != nullptr)
		{
			InputWidget->AddToViewport(); //@ViewPort 삽입
		}
	}
}
