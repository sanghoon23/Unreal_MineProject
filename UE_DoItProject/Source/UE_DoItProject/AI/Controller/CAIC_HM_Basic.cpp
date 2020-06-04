#include "CAIC_HM_Basic.h"
#include "Global.h"

#include "GameFramework/Character.h"
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

	//Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIC_HM_Basic::OnSensingTarget);

	Blackboard->SetValueAsVector("Destination", FVector(0.0f));
}

void ACAIC_HM_Basic::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	//// AI Controller 가 존재하지 않으면,
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

		RunBehaviorTree(BT);
	}
}

void ACAIC_HM_Basic::OnUnPossess()
{
	Super::OnUnPossess();
}

//void ACAIC_HM_Basic::OnSensingTarget(AActor * Actor, FAIStimulus Stimulus)
//{
//	if (Stimulus.WasSuccessfullySensed())
//	{
//		//FString str = Actor->GetName() + FString(L" Success!");
//		//CLog::Print(str, 10.0f, FColor::Red, 10);
//
//		Blackboard->SetValueAsObject("Target", Actor);
//	}
//	else
//	{
//		// CLog::Print(L"Failed", 10.0f, FColor::Red, 10);
//		Blackboard->SetValueAsObject("Target", nullptr);
//	}
//}
