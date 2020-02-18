#include "CPL_SDActionRoll.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"

UCPL_SDActionRoll::UCPL_SDActionRoll()
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

void UCPL_SDActionRoll::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_SDActionRoll::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDActionRoll::OnAction(AActor * DoingActor)
{
	check(DoingActor);

	ACPlayer* Player = Cast<ACPlayer>(DoingActor);
	IfNullRetResult(Player, CLog::Print(L"Roll OnAction Charactor Null!!"));

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade());
	IfTrueRet(Player->IsJumping());
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
