#include "CAIC_HM_Basic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

#include "Interface/IC_Charactor.h"

ACAIC_HM_Basic::ACAIC_HM_Basic()
{
	//SetGenericTeamId(FGenericTeamId(1));

	PatrolRadius = 800.0f;
	DetectRadius = 800.0f;
	AttackRange = 220.0f;

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

	//// AI Controller 가 존재하지 않으면,
	IfNullRet(InPawn->AIControllerClass);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(InPawn);
	IfNullRet(Charactor);

	if (UseBlackboard(BB, Blackboard))
	{
		Blackboard->SetValueAsVector("Home", InPawn->GetActorLocation());
		Blackboard->SetValueAsFloat("PatrolRadius", PatrolRadius);
		Blackboard->SetValueAsFloat("DetectRadius", DetectRadius);
		Blackboard->SetValueAsFloat("AttackRange", AttackRange);

		/* NONE - 초기화 (AIState) */
		//Blackboard->SetValueAsEnum("AIState", static_cast<uint8>(CurrentAIState));

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
}
