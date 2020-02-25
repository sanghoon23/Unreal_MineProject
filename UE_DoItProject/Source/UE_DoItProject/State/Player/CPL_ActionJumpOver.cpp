#include "CPL_ActionJumpOver.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"

UCPL_ActionJumpOver::UCPL_ActionJumpOver()
{
	FString Path = L"";

	// Set Roll Action
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/CommonMon_JumpOver.CommonMon_JumpOver'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> JumpOverAction(*Path);
		if (JumpOverAction.Succeeded())
			JumpOverMontage = JumpOverAction.Object;
	}
}

void UCPL_ActionJumpOver::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_ActionJumpOver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_ActionJumpOver::OnAction()
{
	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////////

	Player->ActorAnimMonPlay(JumpOverMontage, 1.2f, false);
}
