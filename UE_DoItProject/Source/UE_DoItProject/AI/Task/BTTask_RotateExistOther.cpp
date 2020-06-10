#include "BTTask_RotateExistOther.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "NavigationSystem.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"

UBTTask_RotateExistOther::UBTTask_RotateExistOther()
{
	// Name
	NodeName = L"RotateExistOther";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RotateExistOther::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	return EBTNodeResult::Succeeded;
}

void UBTTask_RotateExistOther::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 자기 자신 Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(ControlPawn);

	// Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRet(Target);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRet(World);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector Dir = Target->GetActorLocation() - ControlPawn->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();
	FVector Start = ControlPawn->GetActorLocation();
	FVector End = Start + (Dir * 10000.0f);

	TArray<AActor *> Ignore;
	Ignore.Add(ControlPawn);

	EDrawDebugTrace::Type Debug = EDrawDebugTrace::ForOneFrame;

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), Start, End,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3), //@MonsterAttack
		true, Ignore, Debug, HitResult, true
	);

	//@
	if (bHit == true)
	{
		FVector HitActorLocation = HitResult.GetActor()->GetActorLocation();
		FVector TempDir = HitActorLocation - ControlPawn->GetActorLocation();
		TempDir.Normalize();
		TempDir.Z = 0.0f;

		FVector CrossVec = FVector::CrossProduct(Dir, TempDir);
		CrossVec.Normalize();

		//TODO : Test 하기. - 외적구해서 CharactorMovement Add
		//Controller Orient Movement 쓸지 말지 결정.
		if (CrossVec.Z >= 0)
		{
		}
		else
		{

		}
	}
}
