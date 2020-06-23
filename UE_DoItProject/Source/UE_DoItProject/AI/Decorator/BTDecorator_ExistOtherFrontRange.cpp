#include "BTDecorator_ExistOtherFrontRange.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "AIController.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Monster.h"

UBTDecorator_ExistOtherFrontRange::UBTDecorator_ExistOtherFrontRange()
{
	NodeName = L"ExistOtherFrontRange";
}

bool UBTDecorator_ExistOtherFrontRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 자기 자신 Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, false);

	// Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, false);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRetResult(World, false);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Target 바라보기
	UCFL_ActorAgainst::LookAtTarget(ControlPawn, Target);

	// 위치
	FVector Center = ControlPawn->GetActorLocation();

	// 감지 범위
	float DetectRadius = 100.0f;

	// @Collision Channel 로 인해 CharactorType(Player) 가 감지
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

#if  ENABLE_DRAW_DEBUG
	//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 2.0f);
#endif // DRAW _ DEBUG

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

			if (CrossVec.Z < 0.0f)
			{
				//CLog::Print(ControlPawn->GetName());
				if (DegAngle >= 45.0f && DegAngle < 135.0f)
				{
					return false;
				}
			}
		}
	}

	return true;
}
