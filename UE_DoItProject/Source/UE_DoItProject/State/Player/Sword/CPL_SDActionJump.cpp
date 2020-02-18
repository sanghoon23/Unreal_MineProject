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
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/SwordMon_Jump.SwordMon_Jump'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> jumpMon(*Path);
		if (jumpMon.Succeeded())
			JumpMontage = jumpMon.Object;
	}
}

void UCPL_SDActionJump::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_SDActionJump::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDActionJump::OnAction(AActor * DoingActor)
{
	check(DoingActor);

	ACPlayer* Player = Cast<ACPlayer>(DoingActor);
	IfNullRetResult(Player, CLog::Print(L"SDJump OnAction Charactor Null!!"));

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

