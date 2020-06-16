#include "BTService_BasicDetect.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "AIController.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"

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

	IIC_Monster* I_Monster = Cast<IIC_Monster>(ControlPawn);
	IfNullRet(I_Monster);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRet(World);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 위치
	FVector Center = ControlPawn->GetActorLocation();
	
	// 감지 범위
	float DetectRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("DetectRadius");

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
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 2.0f);
#endif // DRAW _ DEBUG

	// 감지되었다면,
	if (bResult == true)
	{
		for (auto OverlapResult : OverlapResults)
		{
			// @Blackboard - Target 설정
			ACharacter* Charactor = Cast<ACharacter>(OverlapResult.GetActor());
			if (Charactor != nullptr && Charactor->GetController()->IsPlayerController())
			{
				//@Debug
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugLine(World, ControlPawn->GetActorLocation(), Charactor->GetActorLocation(), FColor::Blue, false, 1.0f);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", Charactor);

				//@Target 발견 시 AttackMODE ON
				I_Monster->SetAIAttackMode(true);

				return;
			}
			else
			{
				//@Target 발견 시 AttackMODE OFF
				I_Monster->SetAIAttackMode(false);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
				//OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", 0); //@NONE
			}
		}
	}
	else
	{
		//@Target 발견 시 AttackMODE OFF
		I_Monster->SetAIAttackMode(false);

		OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
		//OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", 0); //@NONE
	}
}

