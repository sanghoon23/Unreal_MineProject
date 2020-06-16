#include "BTService_LookAtTarget.h"
#include "Global.h"

#include "AIController.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

UBTService_LookAtTarget::UBTService_LookAtTarget()
{
	NodeName = L"LookAtTarget";
	Interval = 1.0f;
}

void UBTService_LookAtTarget::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// �ڱ� �ڽ� Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(ControlPawn);

	// Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRet(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Target �ٶ󺸱�
	UCFL_ActorAgainst::LookAtTarget(ControlPawn, Target);

}
