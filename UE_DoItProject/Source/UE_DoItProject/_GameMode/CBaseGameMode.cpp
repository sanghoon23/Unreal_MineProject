#include "CBaseGameMode.h"
#include "Global.h"
#include "Charactor/Player/CPlayer.h"
#include "Charactor/Player/CPlayerController.h"

ACBaseGameMode::ACBaseGameMode()
{
	FString path = L"";

	path = L"Blueprint'/Game/_Mine/_MyBlueprint/Charactor/BpCPlayer.BpCPlayer_C'";
	ConstructorHelpers::FClassFinder<ACPlayer> player(*path);

	if (player.Succeeded())
		DefaultPawnClass = player.Class;

	//
	path = L"Blueprint'/Game/_Mine/_MyBlueprint/Charactor/BpCPlayerController.BpCPlayerController_C'";
	ConstructorHelpers::FClassFinder<ACPlayerController> controller(*path);

	if (controller.Succeeded())
		PlayerControllerClass = controller.Class;
}
