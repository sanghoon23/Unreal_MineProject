#include "CPL_PakrouClimbingLeftMove.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/CInteractActor_Climb.h"

UCPL_PakrouClimbingLeftMove::UCPL_PakrouClimbingLeftMove()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// Set ClimbingIdle Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/PakrouMon_Climbing_MoveLeft.PakrouMon_Climbing_MoveLeft'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbingLeftMoveAction(*Path);
		if (ClimbingLeftMoveAction.Succeeded())
			ClimbingLeftMoveMontage = ClimbingLeftMoveAction.Object;
	}
}

void UCPL_PakrouClimbingLeftMove::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_PakrouClimbingLeftMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// @Warning - SetCurrentBaseAction Setting
void UCPL_PakrouClimbingLeftMove::OnAction()
{
////////////////////////////////////////////////////////////////////////////////////////////////

	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @몽타주 실행
	Player->ActorAnimMonPlay(ClimbingLeftMoveMontage, 1.0f, true);
}

void UCPL_PakrouClimbingLeftMove::BeginActionState()
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

// 1. Left 로 조금씩 움직이기
// 2. Mesh 의 경계 Check
void UCPL_PakrouClimbingLeftMove::TickActionState()
{
	Super::TickActionState();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Convert

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController != nullptr)
	{
		// @Release 
		if (PlayerController->IsInputKeyDown(EKeys::A) == false)
		{
			PakrouActionComp->DoLinkAction(PakrouLinkActionType::IDLE, PakrouObject);
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Player Left Move
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector PlayerLocation = Player->GetActorLocation();
	FVector LeftVec = (-1) * Player->GetActorRightVector();
	PlayerLocation += LeftVec * ClimbingLeftMoveSpeed;

	// @해당 PakrouObject 에서 이동 가능한지 여부 체크
	IfFalseRet(CheckClimbingPossible(PlayerLocation));

	// @가능하면 Set Location
	Player->SetActorLocation(PlayerLocation);
}

void UCPL_PakrouClimbingLeftMove::EndActionState()
{
	Super::EndActionState();

	// @중력 켜기
	Player->OnGravity();

	// @이동 가능
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}
