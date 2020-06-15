#include "BTTask_ExistOther.h"
#include "Global.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "AIController.h"
#include "NavigationSystem.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Monster.h"

UBTTask_ExistOther::UBTTask_ExistOther()
{
	// Name
	NodeName = L"ExistOther";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_ExistOther::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// �ڱ� �ڽ� Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, EBTNodeResult::Failed);

	// Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, EBTNodeResult::Failed);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRetResult(World, EBTNodeResult::Failed);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ��ġ
	FVector Center = ControlPawn->GetActorLocation();

	// ���� ����
	float DetectRadius = 150.0f;

	// @Collision Channel �� ���� CharactorType(Player) �� ����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, ControlPawn);
	// FCollisionResponseParams CollisionResponseParm(ECollisionResponse::ECR_MAX);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, // @TargetSystem
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParm
	);

	// �����Ǿ��ٸ�,
	if (bResult == true)
	{
		for (FOverlapResult& Result : OverlapResults)
		{
			FVector Dir = Result.GetActor()->GetActorLocation() - ControlPawn->GetActorLocation();
			Dir.Z = 0.0f;
			Dir.Normalize();
			FVector RightVec = ControlPawn->GetActorRightVector();
			FVector CrossVec = FVector::CrossProduct(RightVec, Dir);

			//CLog::Print(Result.GetActor()->GetName());
			//CLog::Print(CrossVec);

			float RidAngle = FVector::DotProduct(RightVec, Dir);
			RidAngle = acosf(RidAngle);
			float DegAngle = FMath::RadiansToDegrees(RidAngle);

			if (CrossVec.Z >= 0.0f)
			{
				//CLog::Print(ControlPawn->GetName());
				if (DegAngle >= 45.0f && DegAngle < 135.0f)
					return EBTNodeResult::Failed;
			}
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_ExistOther::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// �ڱ� �ڽ� Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(ControlPawn);

	// Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRet(Target);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRet(World);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ��ġ
	FVector Center = ControlPawn->GetActorLocation();

	// ���� ����
	float DetectRadius = 150.0f;

	// @Collision Channel �� ���� CharactorType(Player) �� ����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, ControlPawn);
	// FCollisionResponseParams CollisionResponseParm(ECollisionResponse::ECR_MAX);
	bool bResult = World->OverlapMultiByChannel
	(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6, // @TargetSystem
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParm
	);

//#if  ENABLE_DRAW_DEBUG
//	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 2.0f);
//#endif // DRAW _ DEBUG

	////@�ڱ��ڽŰ� ���� ����� ������ ����
	//OverlapResults.StableSort([&](const FOverlapResult& A, const FOverlapResult& B)
	//{
	//	float DistanceToA = ControlPawn->GetDistanceTo(A.GetActor());
	//	float DistanceToB = ControlPawn->GetDistanceTo(B.GetActor());

	//	if (DistanceToA < DistanceToB)
	//		return true;
	//	else return false;
	//});

	// �����Ǿ��ٸ�,
	if (bResult == true)
	{
		for (FOverlapResult& Result : OverlapResults)
		{
			FVector Dir = Result.GetActor()->GetActorLocation() - ControlPawn->GetActorLocation();
			Dir.Z = 0.0f;
			Dir.Normalize();
			FVector RightVec = ControlPawn->GetActorRightVector();
			FVector CrossVec = FVector::CrossProduct(RightVec, Dir);

			CLog::Print(Result.GetActor()->GetName());
			CLog::Print(CrossVec);

			float RidAngle = FVector::DotProduct(RightVec, Dir);
			RidAngle = acosf(RidAngle);
			float DegAngle = FMath::RadiansToDegrees(RidAngle);

			if (CrossVec.Z >= 0.0f)
			{
				CLog::Print(ControlPawn->GetName());
				if (DegAngle >= 45.0f && DegAngle < 135.0f)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", 3); //@FINDATTACKPOINT
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
			}
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
