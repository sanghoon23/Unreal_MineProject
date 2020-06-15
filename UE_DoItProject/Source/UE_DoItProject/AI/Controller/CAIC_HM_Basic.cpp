#include "CAIC_HM_Basic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

#include "Interface/IC_Charactor.h"

ACAIC_HM_Basic::ACAIC_HM_Basic()
{
	//SetGenericTeamId(FGenericTeamId(1));

	FString path = L"";

	path = L"BlackboardData'/Game/_Mine/_MyBlueprint/AI/Blackboard/BB_HM_Basic.BB_HM_Basic'";
	ConstructorHelpers::FObjectFinder<UBlackboardData> bb(*path);
	if (bb.Succeeded())
		BB = bb.Object;

	path = L"BehaviorTree'/Game/_Mine/_MyBlueprint/AI/BehaviorTree/BT_HM_Basic.BT_HM_Basic'";
	ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(*path);
	if (bt.Succeeded())
		BT = bt.Object;
}

void ACAIC_HM_Basic::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsVector("Destination", FVector(0.0f));
}

void ACAIC_HM_Basic::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	//// AI Controller �� �������� ������,
	IfNullRet(InPawn->AIControllerClass);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(InPawn);
	IfNullRet(Charactor);

	if (UseBlackboard(BB, Blackboard))
	{
		Blackboard->SetValueAsVector("Home", InPawn->GetActorLocation());
		Blackboard->SetValueAsFloat("PatrolRadius", PatrolRadius);
		Blackboard->SetValueAsFloat("DetectRadius", DetectRadius);
		Blackboard->SetValueAsFloat("HangAround", HangAround);
		Blackboard->SetValueAsFloat("AttackRange", AttackRange);

		/* NONE - �ʱ�ȭ (AIState) */
		Blackboard->SetValueAsEnum("AIState", static_cast<uint8>(CurrentAIState));

		RunBehaviorTree(BT);
	}
}

void ACAIC_HM_Basic::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIC_HM_Basic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//@�ٸ� Pawn ��� �ε����� ���,
	APawn* const ControlPawn = GetPawn();
	check(ControlPawn);
	UWorld* const World = ControlPawn->GetWorld();
	check(World);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	//@Update Distance
//	TArray<int> RemoveIndies;
//	for (int i = 0; i < IgnoreActors.Num(); ++i)
//	{
//		if (ControlPawn->GetDistanceTo(IgnoreActors[i]) > 60.0f)
//		{
//			RemoveIndies.Add(i);
//		}
//	}
//	//@����
//	for (int Index : RemoveIndies)
//	{
//		IgnoreActors.RemoveAt(Index);
//	}
//
//	// ��ġ
//	FVector Center = ControlPawn->GetActorLocation();
//
//	// @Collision Channel �� ���� CharactorType(Player) �� ����
//	TArray<FOverlapResult> OverlapResults;
//	FCollisionQueryParams CollisionQueryParm(NAME_None, false, ControlPawn);
//	CollisionQueryParm.AddIgnoredActors(IgnoreActors); //@
//
//
//	bool bResult = World->OverlapMultiByChannel
//	(
//		OverlapResults,
//		Center,
//		FQuat::Identity,
//		ECollisionChannel::ECC_Visibility, // @Visibility
//		FCollisionShape::MakeSphere(DetectRadius),
//		CollisionQueryParm
//	);
//
//#if  ENABLE_DRAW_DEBUG
//	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 2.0f);
//#endif // DRAW _ DEBUG
//
//	if (bResult == true)
//	{
//		AActor* OverlapActor = OverlapResults[0].GetActor();
//		if (OverlapActor != nullptr)
//		{
//			FVector MineLocation = ControlPawn->GetActorLocation();
//			FVector OverlapLocation = OverlapActor->GetActorLocation();
//
//			FVector Dir = (MineLocation - OverlapLocation);
//			Dir.Normalize();
//
//			//TODO : Test Code
//			FAIMoveRequest Request;
//			Request.SetGoalLocation(MineLocation + (Dir * 1000.0f));
//			MoveTo(Request);
//
//			//Pawn->GetMovementComponent()->AddInputVector(Dir * 50.0f);
//
//
//			CLog::Print(L"AI - MoveTo Call");
//
//			//@IgnoreActor Input
//			IgnoreActors.Add(OverlapActor);
//
//		}
//	}

}
