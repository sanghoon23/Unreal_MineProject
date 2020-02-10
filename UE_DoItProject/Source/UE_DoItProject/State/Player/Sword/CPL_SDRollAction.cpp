#include "CPL_SDRollAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"

UCPL_SDRollAction::UCPL_SDRollAction()
{
	PrimaryComponentTick.bCanEverTick = false;
	//..

	FString Path = L"";

	// Set Roll Action
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MEvade/SD_RollAction_F.SD_RollAction_F'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> RollAction(*Path);
		if (RollAction.Succeeded())
			RollMontage = RollAction.Object;
	}
}

void UCPL_SDRollAction::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_SDRollAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDRollAction::OnAction(AActor * DoingActor)
{
	check(DoingActor);

	ACPlayer* Player = Cast<ACPlayer>(DoingActor);
	IfNullRetResult(Player, CLog::Print(L"OnAction Charactor Null!!"));

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

	/* 우선 charactor Forward 방향으로 */
	Player->SetEvadeSpeed(20.0f);
	Player->SetEvadeDirection(Player->GetActorForwardVector());
	Player->ActorAnimMonPlay(RollMontage, 1.5f, false);
}
