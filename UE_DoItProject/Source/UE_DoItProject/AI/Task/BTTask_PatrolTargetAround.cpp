#include "BTTask_PatrolTargetAround.h"
#include "Global.h"

#include "AIController.h"
#include "NavigationSystem.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"

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
	AAIController* AI_Controller = OwnerComp.GetAIOwner();
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
			// TODO : Target 기준으로 원으로 둘러쌓이게 만들기.
			// 조건 다시 생각해보자.

			FNavLocation CalcVec;
			FVector ActorLocation = overlapResults[0].GetActor()->GetActorLocation();
			Nav->GetRandomReachablePointInRadius(ActorLocation, 300.0f, CalcVec);
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("Origin_PatrolTargetAround", CalcVec);
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//FNavLocation NextLocation;

	////@Origin - IsHangAroundPoint 에서 구한 Owner 의 위치
	//FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector("Origin_PatrolTargetAround");

	//// Random 위치 구하기 후, Succeeded
	//if (Nav->GetRandomReachablePointInRadius(Origin, 100.0f, NextLocation))
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsVector("Destination", NextLocation);
	//	return EBTNodeResult::Succeeded;
	//}

	return EBTNodeResult::Succeeded;
}

void UBTTask_PatrolTargetAround::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
//	// this AI Controller
//	AAIController* AI_Controller = OwnerComp.GetAIOwner();
//	IfNullRet(AI_Controller);
//
//	// this Pawn
//	APawn* Pawn = AI_Controller->GetPawn();
//	IfNullRet(Pawn);
//
//	// Navigation
//	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(Pawn->GetWorld());
//	IfNullRet(Nav);
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CLog::Print(L"PatorlTargetAround TickTas In!!");
//
//	FVector Center = Pawn->GetActorLocation();
//	FCollisionShape Sphere = FCollisionShape::MakeSphere(400.0f);
//	FCollisionQueryParams Params(NAME_None, false, Pawn);
//
//	TArray<FOverlapResult> overlapResults;
//	bool bHit = GetWorld()->OverlapMultiByChannel
//	(
//		overlapResults,
//		Center,
//		FQuat::Identity,
//		ECollisionChannel::ECC_GameTraceChannel6, //@TragetSystem TraceChannel
//		Sphere,
//		Params
//	);
//
//	overlapResults.StableSort([&](const FOverlapResult& A, const FOverlapResult& B)
//	{
//		float DistanceToA = Pawn->GetDistanceTo(A.GetActor());
//		float DistanceToB = Pawn->GetDistanceTo(B.GetActor());
//
//		if (DistanceToA < DistanceToB)
//			return true;
//		else return false;
//	});
//
//	if (bHit == true)
//	{
//		if (Pawn->GetDistanceTo(overlapResults[0].GetActor()) < 200.0f)
//		{
//			FNavLocation CalcVec;
//			FVector ActorLocation = overlapResults[0].GetActor()->GetActorLocation();
//			Nav->GetRandomReachablePointInRadius(ActorLocation, 300.0f, CalcVec);
//			OwnerComp.GetBlackboardComponent()->SetValueAsVector("Origin_PatrolTargetAround", CalcVec);
//		}
//	}
}
