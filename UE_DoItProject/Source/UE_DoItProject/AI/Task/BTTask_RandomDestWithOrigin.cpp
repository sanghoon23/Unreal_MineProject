#include "BTTask_RandomDestWithOrigin.h"
#include "Global.h"

#include "AI/Controller/CAIC_HM_Basic.h"
#include "NavigationSystem.h"

UBTTask_RandomDestWithOrigin::UBTTask_RandomDestWithOrigin()
{
	// Name
	NodeName = L"RandomDestWithOrigin";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_RandomDestWithOrigin::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Navigation
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
