#include "BTTask_PatrolTargetAround.h"
#include "Global.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "AI/Controller/CAIC_HM_Basic.h"
#include "NavigationSystem.h"

UBTTask_PatrolTargetAround::UBTTask_PatrolTargetAround()
{
	// Name
	NodeName = L"PatrolTargetAround";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PatrolTargetAround::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// this AI Controller
	ACAIC_HM_Basic* AI_Controller = Cast<ACAIC_HM_Basic>(OwnerComp.GetAIOwner());
	IfNullRetResult(AI_Controller, EBTNodeResult::Failed);

	// this Pawn
	APawn* Pawn = AI_Controller->GetPawn();
	IfNullRetResult(Pawn, EBTNodeResult::Failed);

	// Navigation
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

	// @Target - Blackboard
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, EBTNodeResult::Failed);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector Center = Pawn->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(400.0f);
	FCollisionQueryParams Params(NAME_None, false, Pawn);

	TArray<FOverlapResult> overlapResults;
	bool bHit = GetWorld()->OverlapMultiByChannel
	(
		overlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, //@TragetSystem TraceChannel
		Sphere,
		Params
	);

	overlapResults.StableSort([&](const FOverlapResult& A, const FOverlapResult& B)
	{
		float DistanceToA = Pawn->GetDistanceTo(A.GetActor());
		float DistanceToB = Pawn->GetDistanceTo(B.GetActor());

		if (DistanceToA < DistanceToB)
			return true;
		else return false;
	});

	if (bHit == true)
	{
		if (Pawn->GetDistanceTo(overlapResults[0].GetActor()) < 200.0f)
		{
			FNavLocation CalcVec;
			FVector ActorLocation = overlapResults[0].GetActor()->GetActorLocation();
			Nav->GetRandomReachablePointInRadius(ActorLocation, 300.0f, CalcVec);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("Origin_PatrolTargetAround", CalcVec);
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FNavLocation NextLocation;

	//@Origin - IsHangAroundPoint 에서 구한 Owner 의 위치
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector("Origin_PatrolTargetAround");

	// Random 위치 구하기 후, Succeeded
	if (Nav->GetRandomReachablePointInRadius(Origin, 100.0f, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("Destination", NextLocation);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
