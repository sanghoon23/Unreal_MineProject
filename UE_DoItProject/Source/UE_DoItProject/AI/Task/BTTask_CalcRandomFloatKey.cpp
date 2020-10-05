#include "BTTask_CalcRandomFloatKey.h"
#include "Kismet/KismetMathLibrary.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"

#include "CLog.h"

UBTTask_CalcRandomFloatKey::UBTTask_CalcRandomFloatKey()
{
	NodeName = L"CalcRandomFloatKey";

	bNotifyTick = false; // Tick True;
}

EBTNodeResult::Type UBTTask_CalcRandomFloatKey::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FRandomStream Stream;
	Stream.GenerateNewSeed();
	float fRandom = UKismetMathLibrary::RandomFloatInRangeFromStream(0.000f, 100.000f, Stream);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("RandomFloatKey", fRandom);

	return EBTNodeResult::Succeeded;
}

