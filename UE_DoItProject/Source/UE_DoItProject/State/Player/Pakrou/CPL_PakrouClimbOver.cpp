#include "CPL_PakrouClimbOver.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/CInteractActor_Climb.h"

UCPL_PakrouClimbOver::UCPL_PakrouClimbOver()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// Set ClimbingIdle Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/PakrouMon_ClimbOver.PakrouMon_ClimbOver'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbOverAction(*Path);
		if (ClimbOverAction.Succeeded())
			ClimbOverMontage = ClimbOverAction.Object;
	}
}

void UCPL_PakrouClimbOver::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_PakrouClimbOver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// @Warning - SetCurrentBaseAction Setting
void UCPL_PakrouClimbOver::OnAction()
{
	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @몽타주 실행
	Player->ActorAnimMonPlay(ClimbOverMontage, 1.0f, true);
}

void UCPL_PakrouClimbOver::BeginActionState()
{
	Super::BeginActionState();

	// @중력 끄기
	Player->OffGravity();

	// @속력 줄이기 - 중력끄고 바로 해줘야함
	Player->GetCharacterMovement()->Velocity = FVector(0.0f);

	// @이동 불가
	Player->CanNotMove();

	// @Warning - KeyInput 불가
	Player->OnBlockKeyInput();
}

void UCPL_PakrouClimbOver::TickActionState()
{
	Super::TickActionState();
}

void UCPL_PakrouClimbOver::EndActionState()
{
	Super::EndActionState();

	// @중력 켜기
	Player->OnGravity();

	// @이동 가능
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}
