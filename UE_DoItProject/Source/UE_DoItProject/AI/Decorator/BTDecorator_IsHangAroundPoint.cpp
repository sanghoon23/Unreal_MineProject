#include "BTDecorator_IsHangAroundPoint.h"
#include "Global.h"

#include "AI/Controller/CAIC_HM_Basic.h"

UBTDecorator_IsHangAroundPoint::UBTDecorator_IsHangAroundPoint()
{
	NodeName = L"IsHangAroundPoint";
}

bool UBTDecorator_IsHangAroundPoint::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, false);

	APawn* Target
		= Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, false);

	// @���� ���� ������ �������� �ʾҴٸ� ����,
	float HangAround = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("HangAround");

	if (ControlPawn->GetDistanceTo(Target) < HangAround)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("Origin_PatrolTargetAround", ControlPawn->GetActorLocation());
		return true;
	}

	return false; //@ �̶�, Target ���� �̵��Ѵ�.
}
