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

	// @��Ÿ�� ����
	Player->ActorAnimMonPlay(ClimbingLeftMoveMontage, 1.0f, true);
}

void UCPL_PakrouClimbingLeftMove::BeginActionState()
{
	Super::BeginActionState();

	// @�߷� ����
	Player->OffGravity();

	// @�ӷ� ���̱� - �߷²��� �ٷ� �������
	Player->GetCharacterMovement()->Velocity = FVector(0.0f);

	// @�̵� �Ұ�
	Player->CanNotMove();

	// @Warning - KeyInput �Ұ�
	Player->OnBlockKeyInput();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

// 1. Left �� ���ݾ� �����̱�
// 2. Mesh �� ��� Check
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

	// @�ش� PakrouObject ���� �̵� �������� ���� üũ
	IfFalseRet(CheckClimbingPossible(PlayerLocation));

	// @�����ϸ� Set Location
	Player->SetActorLocation(PlayerLocation);
}

void UCPL_PakrouClimbingLeftMove::EndActionState()
{
	Super::EndActionState();

	// @�߷� �ѱ�
	Player->OnGravity();

	// @�̵� ����
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}
