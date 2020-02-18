#include "CPL_MGActionJump.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"

UCPL_MGActionJump::UCPL_MGActionJump()
{
	PrimaryComponentTick.bCanEverTick = false;
	//..

	FString Path = L"";

	// Set Roll Action
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MageMon_Jump.MageMon_Jump'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> jumpMon(*Path);
		if (jumpMon.Succeeded())
			JumpMontage = jumpMon.Object;
	}
}

void UCPL_MGActionJump::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_MGActionJump::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_MGActionJump::OnAction(AActor * DoingActor)
{
	check(DoingActor);

	ACPlayer* Player = Cast<ACPlayer>(DoingActor);
	IfNullRetResult(Player, CLog::Print(L"MGJump OnAction Charactor Null!!"));

	IIC_BaseAttack* BaseAttack = Player->GetIAttackComp()->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		IfTrueRet(BaseAttack->GetAttacking()); // @IF TRUE RETURN
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade());
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

	//@ ½ÇÇà
	Player->SetJumping(true);
	Player->Jump();
	Player->ActorAnimMonPlay(JumpMontage, 0.8f, false);
}

