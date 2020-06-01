#include "CPL_PakrouClimbingRightMove.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/CInteractActor_Climb.h"

UCPL_PakrouClimbingRightMove::UCPL_PakrouClimbingRightMove()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// Set ClimbingIdle Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/PakrouMon_Climbing_MoveRight.PakrouMon_Climbing_MoveRight'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbingRightMoveAction(*Path);
		if (ClimbingRightMoveAction.Succeeded())
			ClimbingRightMoveMontage = ClimbingRightMoveAction.Object;
	}
}

void UCPL_PakrouClimbingRightMove::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);
}

void UCPL_PakrouClimbingRightMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// @Warning - SetCurrentBaseAction Setting
void UCPL_PakrouClimbingRightMove::OnAction()
{
////////////////////////////////////////////////////////////////////////////////////////////////

	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @��Ÿ�� ����
	Player->ActorAnimMonPlay(ClimbingRightMoveMontage, 1.0f, true);

	//@Looping
	Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection
	(
		FName("Loop"), ClimbingRightMoveMontage
	);
}

void UCPL_PakrouClimbingRightMove::BeginActionState()
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
}

void UCPL_PakrouClimbingRightMove::TickActionState()
{
	Super::TickActionState();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Convert

	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController != nullptr)
	{
		// @Release 
		if (PlayerController->IsInputKeyDown(EKeys::D) == false)
		{
			PakrouActionComp->DoLinkAction(PakrouLinkActionType::IDLE, PakrouObject);
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Player Right Move

	FVector PlayerLocation = Player->GetActorLocation();
	FVector LeftVec = Player->GetActorRightVector();
	PlayerLocation += LeftVec * ClimbingRightMoveSpeed;

	// @�ش� PakrouObject ���� �̵� �������� ���� üũ
	IfFalseRet(CheckClimbingPossible(PlayerLocation));

	// @�����ϸ� Set Location
	Player->SetActorLocation(PlayerLocation);
}

void UCPL_PakrouClimbingRightMove::EndActionState()
{
	Super::EndActionState();

	// @�߷� �ѱ�
	Player->OnGravity();

	// @�̵� ����
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}
