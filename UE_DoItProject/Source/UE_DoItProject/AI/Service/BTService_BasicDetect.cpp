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

/* AI Detect Trace Type 을 이용해 Charactor Object Type Channel(Plyaer) 를 감지 */
void UBTService_BasicDetect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 자기 자신 Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(ControlPawn);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRet(World);

	// 위치
	FVector Center = ControlPawn->GetActorLocation();
	
	// 감지 범위
	float DetectRadius = 700.0f;

	// @Collision Channel 로 인해 CharactorType(Player) 가 감지
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

	// 감지되었다면,
	if (bResult == true)
	{
		for (auto OverlapResult : OverlapResults)
		{
			// Test Code
			// CLog::Print(OverlapResult.GetActor()->GetName());

			// @Blackboard - Target 설정
			APawn* Charactor = Cast<APawn>(OverlapResult.GetActor());
			if (Charactor != nullptr && Charactor->GetController()->IsPlayerController())
			{
				// CLog::Print(L"Good!!");
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", Charactor);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControlPawn->GetActorLocation(), Charactor->GetActorLocation(), FColor::Blue, false, 1.0f);
				return;
			}
			else
			{
				// CLog::Print(L"Bad!!");
				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
			}
		}
	}
	else
	{
		// CLog::Print(L"Bad!!");
		OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
	}
}

