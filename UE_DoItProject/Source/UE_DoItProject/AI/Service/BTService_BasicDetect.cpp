#include "BTService_BasicDetect.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"

#include "AI/Controller/CAIC_HM_Basic.h"
#include "Charactor/Monster/CHM_Basic.h"

UBTService_BasicDetect::UBTService_BasicDetect()
{
	NodeName = L"Detect";
	Interval = 1.0f;
}

/* AI Detect Trace Type �� �̿��� Charactor Object Type Channel(Plyaer) �� ���� */
void UBTService_BasicDetect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// �ڱ� �ڽ� Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(ControlPawn);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRet(World);

	// ��ġ
	FVector Center = ControlPawn->GetActorLocation();
	
	// ���� ����
	float DetectRadius = 700.0f;

	// @Collision Channel �� ���� CharactorType(Player) �� ����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, ControlPawn);
	// FCollisionResponseParams CollisionResponseParm(ECollisionResponse::ECR_MAX);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel1, // @AI Detect (TraceType)
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 2.0f);
#endif // DRAW _ DEBUG

	// �����Ǿ��ٸ�,
	if (bResult == true)
	{
		for (auto OverlapResult : OverlapResults)
		{
			// @Blackboard - Target ����
			ACharacter* Charactor = Cast<ACharacter>(OverlapResult.GetActor());
			if (Charactor != nullptr && Charactor->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", Charactor);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControlPawn->GetActorLocation(), Charactor->GetActorLocation(), FColor::Blue, false, 1.0f);
				return;
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
	}
}

