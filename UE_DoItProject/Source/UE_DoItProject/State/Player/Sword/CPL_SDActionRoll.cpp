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
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MEvade/SwordMon_RollAction.SwordMon_RollAction'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> RollAction(*Path);
		if (RollAction.Succeeded())
			RollMontage = RollAction.Object;
	}
}

void UCPL_SDActionRoll::BeginPlay()
{
	Super::BeginPlay();
	
	// @Set PlayerActor
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_SDActionRoll::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDActionRoll::OnAction()
{
	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	//@타겟 구하기.
	AActor* Target = Player->GetFindAttackTarget();
	IfNullRet(Target);

	//@타겟과 거리 체크.
	if (Player->GetDistanceTo(Target) > UsingActionRange)
	{
		UE_LOG(LogTemp, Warning, L"Distance from Target!!");
		return;
	}

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////////

	//@타겟 바라보기.
	LookAtTarget(Target);

	// @Set Evade
	Player->SetEvade(true);
	Player->SetEvadeSpeed(11.0f);
	
	FVector BackVector = (-1) * Player->GetActorForwardVector();
	Player->SetEvadeDirection(BackVector);
	Player->ActorAnimMonPlay(RollMontage, 1.0f, true);

	//
	//APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	//if (Controller != nullptr)
	//{
	//	if (Controller->IsInputKeyDown(EKeys::W))
	//	{
	//		SetAngleWithControlRot(0.0f);
	//	}
	//	else if (Controller->IsInputKeyDown(EKeys::S))
	//	{
	//		SetAngleWithControlRot(180.0f);
	//	}
	//	else if (Controller->IsInputKeyDown(EKeys::A))
	//	{
	//		SetAngleWithControlRot(-90.0f);
	//	}
	//	else if (Controller->IsInputKeyDown(EKeys::D))
	//	{
	//		SetAngleWithControlRot(90.0f);
	//	}
	//}

}

void UCPL_SDActionRoll::SetAngleWithControlRot(float Angle)
{
	FRotator ControlRotation = Player->GetControlRotation();
	FRotator Y = FRotator(0.0f, ControlRotation.Yaw + Angle, 0.0f);
	FVector Right = FQuat(Y).GetRightVector();

	Player->SetActorRotation(Y);
}

void UCPL_SDActionRoll::LookAtTarget(AActor * Target)
{
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}
