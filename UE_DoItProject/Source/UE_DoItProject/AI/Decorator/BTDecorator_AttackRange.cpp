#include "BTDecorator_AttackRange.h"
#include "Global.h"

#include "AI/Controller/CAIC_HM_Basic.h"

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = L"AttackRange";
}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, false);

	APawn* Target
		= Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, false);

	Result = (Target->GetDistanceTo(ControlPawn) <= AttackRange);
	return Result;
}
