#include "HUD_Main.h"
#include "ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

AHUD_Main::AHUD_Main()
{
	FString strPath = L"";

	#pragma region Skill Casting Bar
	//Skill Casting Bar
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_SkillCastingBar.BpCWG_SkillCastingBar_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> CastingClass(*strPath);
	if (CastingClass.Succeeded())
	{
		CastingWidgetClass = CastingClass.Class;
	}

	#pragma endregion

	#pragma region Target Info
	//Target Info
	strPath = L"WidgetBlueprint'/Game/_Mine/_MyBlueprint/Widget/BpCWG_TargetInfo.BpCWG_TargetInfo_C'";
	ConstructorHelpers::FClassFinder<UUserWidget> TargetInfoClass(*strPath);
	if (TargetInfoClass.Succeeded())
	{
		TargetInfoWidgetClass = TargetInfoClass.Class;
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

	//@TargetInfo Widget Widget
	if (TargetInfoWidgetClass != nullptr)
	{
		TargetInfoWidget = CreateWidget<UWG_TargetInfo>(GetWorld(), TargetInfoWidgetClass);
		if (TargetInfoWidget)
		{
			TargetInfoWidget->SetVisibility(ESlateVisibility::Hidden);
			TargetInfoWidget->AddToViewport();
		}
	}
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
			InputWidget->AddToViewport(); //@ViewPort »ðÀÔ
		}
	}
}
