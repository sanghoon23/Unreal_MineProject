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

	// @��Ÿ�� ����
	Player->ActorAnimMonPlay(ClimbOverMontage, 1.0f, true);
}

void UCPL_PakrouClimbOver::BeginActionState()
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

void UCPL_PakrouClimbOver::TickActionState()
{
	Super::TickActionState();
}

void UCPL_PakrouClimbOver::EndActionState()
{
	Super::EndActionState();

	// @�߷� �ѱ�
	Player->OnGravity();

	// @�̵� ����
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}
