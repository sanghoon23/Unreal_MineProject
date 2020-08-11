#include "BTTask_FindPatrol.h"
#include "Global.h"

#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_FindPatrol::UBTTask_FindPatrol()
{
	// Name
	NodeName = L"FindPatrol";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_FindPatrol::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// this Controller
	AAIController* controller = OwnerComp.GetAIOwner();
	IfNullRetResult(controller, EBTNodeResult::Failed);

	// this Pawn
	APawn* pawn = controller->GetPawn();
	IfNullRetResult(pawn, EBTNodeResult::Failed);

	//CLog::Print(L"AI FindPatrol In!!");

	// Navigation
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

	//CLog::Print(L"AI FindPatrol RetResult In!!");

	// Blackboard - Home & PatrolRadius
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector("Home");
	FNavLocation NextLocation;

	// @PatrolRadius
	float PatrolRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("PatrolRadius");

	// Random 위치 구하기 후, Succeeded
	if (Nav->GetRandomPointInNavigableRadius(Origin, PatrolRadius, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("Destination", NextLocation);

		//CLog::Print(L"Destination Insert Succeed!!");

		return EBTNodeResult::Succeeded;
	}

	CLog::Print(L"Destination Insert Failed!!");

	return EBTNodeResult::Failed;
}
