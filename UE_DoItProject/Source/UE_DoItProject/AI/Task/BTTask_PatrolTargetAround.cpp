#include "BTTask_PatrolTargetAround.h"
#include "Global.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "AI/Controller/CAIC_HM_Basic.h"
#include "NavigationSystem.h"

UBTTask_PatrolTargetAround::UBTTask_PatrolTargetAround()
{
	// Name
	NodeName = L"PatrolTargetAround";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PatrolTargetAround::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// this AI Controller
	ACAIC_HM_Basic* AI_Controller = Cast<ACAIC_HM_Basic>(OwnerComp.GetAIOwner());
	IfNullRetResult(AI_Controller, EBTNodeResult::Failed);

	// this Pawn
	APawn* Pawn = AI_Controller->GetPawn();
	IfNullRetResult(Pawn, EBTNodeResult::Failed);

	// Navigation
	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

	// @Target - Blackboard
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, EBTNodeResult::Failed);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////@Calc - Destination
	//FVector Dir = Target->GetActorLocation() - Pawn->GetActorLocation();
	//Dir.Normalize();

	//FVector CrossVec = FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), Dir);
	//CrossVec.Normalize();

	//float Inner = 0.0f;
	////@오른쪽에 존재한다면,
	//if (CrossVec.Z >= 0)
	//{
	//	Inner = Dir | FVector(1.0f, 0.0f, 0.0f);
	//}
	//else
	//{
	//	Inner = Dir | FVector(1.0f, 0.0f, 0.0f);
	//	Inner *= (-1);
	//}


	//float Radian = FMath::Acos(Inner);
	//float Angle = FMath::RadiansToDegrees(Radian);

	////CLog::Print(Angle);

	//bRandomDir = !bRandomDir;
	//(bRandomDir)
	//	? Angle += 10.0f
	//	: Angle -= 10.0f;

	////aCLog::Print(Angle);

	//FRotator Rotate = FRotator(0.0f, Angle, 0.0f);
	//FVector Temp = FQuat(Rotate).GetForwardVector();

	//float AroundDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AroundDistanceToTarget");
	//Temp *= AroundDistance;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FNavLocation NextLocation;

	//@Origin - IsHangAroundPoint 에서 구한 Owner 의 위치
	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector("Origin_PatrolTargetAround");

	// Random 위치 구하기 후, Succeeded
	if (Nav->GetRandomPointInNavigableRadius(Origin, 100.0f, NextLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("Destination", NextLocation);
		return EBTNodeResult::Succeeded;
	}

	////@Result
	//NextLocation = Temp;
	//NextLocation.Z = Pawn->GetActorLocation().Z;

	//CLog::Print(NextLocation);

	////@Set
	//OwnerComp.GetBlackboardComponent()->SetValueAsVector("Destination", NextLocation);

	//@Target 바라보기
	UCFL_ActorAgainst::LookAtTarget(OwnerComp.GetAIOwner(), Target);

	return EBTNodeResult::Failed;
}
