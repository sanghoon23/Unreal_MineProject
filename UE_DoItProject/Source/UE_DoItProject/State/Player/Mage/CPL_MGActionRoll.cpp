#include "CPL_MGActionRoll.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"

UCPL_MGActionRoll::UCPL_MGActionRoll()
{
	PrimaryComponentTick.bCanEverTick = false;
	//..

	FString Path = L"";

	// Set Roll Action
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MEvade/MG_RollAction_F.MG_RollAction_F'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> RollAction(*Path);
		if (RollAction.Succeeded())
			RollMontage = RollAction.Object;
	}
}

void UCPL_MGActionRoll::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_MGActionRoll::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MGActionRoll::OnAction()
{
	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////////

	// @Set Evade
	Player->SetEvade(true);

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (Controller != nullptr)
	{
		if (Controller->IsInputKeyDown(EKeys::W))
		{
			SetAngleWithControlRot(0.0f);
		}
		else if (Controller->IsInputKeyDown(EKeys::S))
		{
			SetAngleWithControlRot(180.0f);
		}
		else if (Controller->IsInputKeyDown(EKeys::A))
		{
			SetAngleWithControlRot(-90.0f);
		}
		else if (Controller->IsInputKeyDown(EKeys::D))
		{
			SetAngleWithControlRot(90.0f);
		}
	}

	/* 우선 charactor Forward 방향으로 */
	Player->SetEvadeSpeed(5.0f);
	Player->SetEvadeDirection(Player->GetActorForwardVector());
	Player->ActorAnimMonPlay(RollMontage, 1.5f, true);
}

void UCPL_MGActionRoll::SetAngleWithControlRot(float Angle)
{
	FRotator ControlRotation = Player->GetControlRotation();
	FRotator Y = FRotator(0.0f, ControlRotation.Yaw + Angle, 0.0f);
	FVector Right = FQuat(Y).GetRightVector();

	Player->SetActorRotation(Y);
}
