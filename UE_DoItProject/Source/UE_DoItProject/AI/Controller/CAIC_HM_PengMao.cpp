#include "CAIC_HM_PengMao.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "NavigationSystem.h"

#include "Interface/IC_Charactor.h"

ACAIC_HM_PengMao::ACAIC_HM_PengMao()
{
	FString path = L"";

	path = L"BlackboardData'/Game/_Mine/_MyBlueprint/AI/Blackboard/BB_HM_PengMao.BB_HM_PengMao'";
	ConstructorHelpers::FObjectFinder<UBlackboardData> bb(*path);
	if (bb.Succeeded())
		BB = bb.Object;

	path = L"BehaviorTree'/Game/_Mine/_MyBlueprint/AI/BehaviorTree/BT_HM_PengMao.BT_HM_PengMao'";
	ConstructorHelpers::FObjectFinder<UBehaviorTree> bt(*path);
	if (bt.Succeeded())
		BT = bt.Object;
}

void ACAIC_HM_PengMao::BeginPlay()
{
	Super::BeginPlay();

	Blackboard->SetValueAsVector("Destination", FVector(0.0f));
}

void ACAIC_HM_PengMao::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	// AI Controller 가 존재하지 않으면,
	IfNullRet(InPawn->AIControllerClass);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(InPawn);
	IfNullRet(Charactor);

	if (UseBlackboard(BB, Blackboard))
	{
		Blackboard->SetValueAsVector("Home", InPawn->GetActorLocation());
		Blackboard->SetValueAsFloat("PatrolRadius", PatrolRadius);
		Blackboard->SetValueAsFloat("DetectRadius", DetectRadius);
		Blackboard->SetValueAsFloat("AttackRange", AttackRange);

		RunBehaviorTree(BT);
	}
}

void ACAIC_HM_PengMao::OnUnPossess()
{
	Super::OnUnPossess();
}
