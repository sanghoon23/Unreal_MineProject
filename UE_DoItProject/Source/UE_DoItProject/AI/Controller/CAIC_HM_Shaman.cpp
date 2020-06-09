#include "CAIC_HM_Shaman.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "NavigationSystem.h"

#include "Interface/IC_Charactor.h"

ACAIC_HM_Shaman::ACAIC_HM_Shaman()
{
	FString path = L"";

	path = L"BlackboardData'/Game/_Mine/_MyBlueprint/AI/Blackboard/BB_HM_Shaman.BB_HM_Shaman'";
	ConstructorHelpers::FObjectFinder<UBlackboardData> bb(*path);
	if (bb.Succeeded())
		BB = bb.Object;

	path = L"BehaviorTree'/Game/_Mine/_MyBlueprint/AI/BehaviorTree/BT_HM_Shaman.BT_HM_Shaman'";
	ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(*path);
	if (bt.Succeeded())
		BT = bt.Object;
}

void ACAIC_HM_Shaman::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsVector("Destination", FVector(0.0f));
}

void ACAIC_HM_Shaman::OnPossess(APawn * InPawn)
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

		/* NONE - 초기화 (AIState) */
		Blackboard->SetValueAsEnum("AIState", static_cast<uint8>(InputFirstAIState));

		RunBehaviorTree(BT);
	}
}

void ACAIC_HM_Shaman::OnUnPossess()
{
	Super::OnUnPossess();
}
