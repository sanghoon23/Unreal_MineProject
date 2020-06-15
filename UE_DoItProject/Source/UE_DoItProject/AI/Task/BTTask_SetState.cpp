#include "BTTask_SetState.h"
#include "Global.h"

#include "AIController.h"
#include "NavigationSystem.h"

UBTTask_SetState::UBTTask_SetState()
{
	// Name
	NodeName = L"SetState";

	// @Tick
	bNotifyTick = true;
}


EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(AIStateEntryName, static_cast<uint8>(EnumValue));

	return EBTNodeResult::Succeeded;
}
