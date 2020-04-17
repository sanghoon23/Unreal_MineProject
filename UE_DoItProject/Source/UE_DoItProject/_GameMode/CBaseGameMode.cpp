#include "CBaseGameMode.h"
#include "Global.h"
#include "Charactor/Player/CPlayer.h"
#include "Charactor/Player/CPlayerController.h"
#include "UI/HUD_Main.h"

ACBaseGameMode::ACBaseGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	FString path = L"";

	//@PawnClass
	path = L"Blueprint'/Game/_Mine/_MyBlueprint/Charactor/BpCPlayer.BpCPlayer_C'";
	ConstructorHelpers::FClassFinder<ACPlayer> player(*path);
	if (player.Succeeded())
		DefaultPawnClass = player.Class;

	//@PC
	path = L"Blueprint'/Game/_Mine/_MyBlueprint/Charactor/BpCPlayerController.BpCPlayerController_C'";
	ConstructorHelpers::FClassFinder<ACPlayerController> controller(*path);
	if (controller.Succeeded())
		PlayerControllerClass = controller.Class;

	//@HUD
	HUDClass = AHUD_Main::StaticClass();
}

void ACBaseGameMode::BeginPlay()
{
	Super::BeginPlay();

	//@Set GameState
	CurrentMyGamePlayState = EMyGamePlayState::EPlaying;

	////@Set Widget
	//SetCurrentWidget(StartWidgetClass);
}

void ACBaseGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void ACBaseGameMode::SetCurrentWidget(TSubclassOf<UUserWidget> InputWidget)
//{
//	if (CurrentWidget != nullptr)
//	{
//		CurrentWidget->RemoveFromViewport();
//		CurrentWidget = nullptr;
//	}
//
//	if (InputWidget != nullptr)
//	{
//		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), InputWidget);
//		if (CurrentWidget != nullptr)
//		{
//			CurrentWidget->AddToViewport(); //@ViewPort »ðÀÔ
//		}
//	}
//}

///////////////////////////////////////////////////////////////////////////////////////////////////

EMyGamePlayState ACBaseGameMode::GetMyGamePlayState() const
{
	return CurrentMyGamePlayState;
}
