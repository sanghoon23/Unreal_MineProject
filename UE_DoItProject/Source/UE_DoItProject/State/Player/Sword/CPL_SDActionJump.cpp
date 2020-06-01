#include "CPL_SDActionJump.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"

UCPL_SDActionJump::UCPL_SDActionJump()
{
	PrimaryComponentTick.bCanEverTick = false;
	//..

	FString Path = L"";

	// Set Roll Action
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/SwordMon_JumpStart.SwordMon_JumpStart'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> jumpMon(*Path);
		if (jumpMon.Succeeded())
			JumpMontage = jumpMon.Object;
	}
}

void UCPL_SDActionJump::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(GetOwner());
	check(Player);
}

void UCPL_SDActionJump::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDActionJump::OnAction()
{
	IIC_BaseAttack* BaseAttack = Player->GetIAttackComp()->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		IfTrueRet(BaseAttack->GetAttacking()); // @IF TRUE RETURN
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@IsFalling
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

	//@ ½ÇÇà
	Player->SetJumping(true);
	Player->Jump();
	Player->ActorAnimMonPlay(JumpMontage, 0.5f, false);
}

