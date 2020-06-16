#include "BTDecorator_CheckHomeRange.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "AIController.h"

UBTDecorator_CheckHomeRange::UBTDecorator_CheckHomeRange()
{
	NodeName = L"CheckHomeRange";
}

bool UBTDecorator_CheckHomeRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 자기 자신 Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, false);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector HomePos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("Home");
	FVector Distance = ControlPawn->GetActorLocation() - HomePos;

	return Distance.Size2D() > RangeDistance;
}

