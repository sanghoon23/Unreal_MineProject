#include "BTTask_TurnToTarget.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "AI/Controller/CAIC_HM_Basic.h"
#include "NavigationSystem.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	// Name
	NodeName = L"FindPatrol";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// @해당 AI Controller 를 가지고 있는 Pawn
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(Pawn, EBTNodeResult::Failed);

	// @BlackBoardComponent Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, EBTNodeResult::Failed);

	FRotator Current = Pawn->GetActorRotation();
	FVector DestVec = Target->GetActorLocation() - Pawn->GetActorLocation();
	DestVec.Z = 0.0f;
	FRotator DestRot = FRotationMatrix::MakeFromX(DestVec).Rotator();

	Pawn->SetActorRotation
	(
		FMath::RInterpTo(Current, DestRot, GetWorld()->DeltaTimeSeconds, 2.0f)
	);

	return EBTNodeResult::Succeeded;
}


