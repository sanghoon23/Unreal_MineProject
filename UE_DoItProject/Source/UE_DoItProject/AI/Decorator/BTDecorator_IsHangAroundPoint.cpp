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

	// @계산된 도착 지점에 도착하지 않았다면 리턴,
	float HangAround = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("HangAround");
	float DetectRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("DetectRadius");

	if (ControlPawn->GetDistanceTo(Target) <= HangAround)
	{
		FVector Dir = ControlPawn->GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();
		float RandomDistance = FMath::RandRange(10.0f, HangAround);
		FVector Dest = ControlPawn->GetActorLocation() + (Dir * (RandomDistance));

		OwnerComp.GetBlackboardComponent()->SetValueAsVector("Origin_PatrolTargetAround", Dest);
		return true;
	}
	else
	{
		FVector Dir = Target->GetActorLocation() - ControlPawn->GetActorLocation();
		Dir.Normalize();
		FVector Dest = ControlPawn->GetActorLocation() + (Dir * (DetectRadius - HangAround));

		//@Set Destination
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("Destination", Dest);

		return false;
	}
}
