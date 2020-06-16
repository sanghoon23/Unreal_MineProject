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

/* AI Detect Trace Type �� �̿��� Charactor Object Type Channel(Plyaer) �� ���� */
void UBTService_BasicDetect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// �ڱ� �ڽ� Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(ControlPawn);

	IIC_Monster* I_Monster = Cast<IIC_Monster>(ControlPawn);
	IfNullRet(I_Monster);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRet(World);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ��ġ
	FVector Center = ControlPawn->GetActorLocation();
	
	// ���� ����
	float DetectRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("DetectRadius");

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
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 2.0f);
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
				//@Debug
				//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugLine(World, ControlPawn->GetActorLocation(), Charactor->GetActorLocation(), FColor::Blue, false, 1.0f);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", Charactor);

				//@Target �߰� �� AttackMODE ON
				I_Monster->SetAIAttackMode(true);

				return;
			}
			else
			{
				//@Target �߰� �� AttackMODE OFF
				I_Monster->SetAIAttackMode(false);

				OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
				//OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", 0); //@NONE
			}
		}
	}
	else
	{
		//@Target �߰� �� AttackMODE OFF
		I_Monster->SetAIAttackMode(false);

		OwnerComp.GetBlackboardComponent()->SetValueAsObject("Target", nullptr);
		//OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", 0); //@NONE
	}
}

