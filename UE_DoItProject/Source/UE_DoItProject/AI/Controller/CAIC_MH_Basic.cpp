#include "CAIC_MH_Basic.h"
#include "Global.h"

#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Interface/IC_Charactor.h"

ACAIC_MH_Basic::ACAIC_MH_Basic()
{
	SetGenericTeamId(FGenericTeamId(1));

	FString path = L"BlackboardData'/Game/_Mine/_MyBlueprint/AI/Blackboard/BB_MH_Basic.BB_MH_Basic'";
	static ConstructorHelpers::FObjectFinder<UBlackboardData> bb(*path);
	if (bb.Succeeded())
		BB = bb.Object;

	path = L"BehaviorTree'/Game/_Mine/_MyBlueprint/AI/BehaviorTree/BT_MH_Basic.BT_MH_Basic'";
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(*path);
	if (bt.Succeeded())
		BT = bt.Object;

	// AI Perception Component
	{
		Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

		Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
		Sight->SightRadius = 400.0f;
		Sight->LoseSightRadius = 500.0f;
		Sight->PeripheralVisionAngleDegrees = 45.0f;
		Sight->SetMaxAge(5.0f);

		Sight->DetectionByAffiliation.bDetectEnemies = true;
		Sight->DetectionByAffiliation.bDetectFriendlies = false;
		Sight->DetectionByAffiliation.bDetectNeutrals = false;

		Perception->ConfigureSense(*Sight);
		Perception->SetDominantSense(*Sight->GetSenseImplementation());
	}

}

void ACAIC_MH_Basic::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIC_MH_Basic::OnSensingTarget);
}

void ACAIC_MH_Basic::OnPossess(APawn * InPawn)
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

		RunBehaviorTree(BT);
	}
}

void ACAIC_MH_Basic::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIC_MH_Basic::OnSensingTarget(AActor * Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		//FString str = Actor->GetName() + FString(L" Success!");
		//CLog::Print(str, 10.0f, FColor::Red, 10);

		Blackboard->SetValueAsObject("Target", Actor);
	}
	else
	{
		// CLog::Print(L"Failed", 10.0f, FColor::Red, 10);
		Blackboard->SetValueAsObject("Target", nullptr);
	}
}
