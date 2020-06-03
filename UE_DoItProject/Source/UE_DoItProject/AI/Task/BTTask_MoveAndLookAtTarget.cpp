#include "BTTask_MoveAndLookAtTarget.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "AI/Controller/CAIC_HM_Basic.h"
#include "NavigationSystem.h"

UBTTask_MoveAndLookAtTarget::UBTTask_MoveAndLookAtTarget()
{
	// Name
	NodeName = L"MoveAndLookAtTarget";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_MoveAndLookAtTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// this Controller
	ACAIC_HM_Basic* Controller = Cast<ACAIC_HM_Basic>(OwnerComp.GetAIOwner());
	IfNullRetResult(Controller, EBTNodeResult::Failed);

	// this Pawn
	APawn* Pawn = Controller->GetPawn();
	IfNullRetResult(Pawn, EBTNodeResult::Failed);

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, EBTNodeResult::Failed);

	//@Target �ٶ󺸱�
	UCFL_ActorAgainst::LookAtTarget(Pawn, Target);

	FVector Location = Pawn->GetActorLocation();
	FVector Dest = OwnerComp.GetBlackboardComponent()->GetValueAsVector("Destination");

	FVector Dir = Location - Dest;
	Dir.Normalize();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	Location += Dir * DeltaTime * 5.0f; /* Speed */

	//@Set
	FAIMoveRequest Request;
	Request.SetGoalLocation(Location);

	Controller->MoveTo(Request);

	return EBTNodeResult::Succeeded;
}
