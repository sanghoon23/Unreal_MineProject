#include "BTDecorator_CheckRandPercent.h"
#include "Kismet/KismetMathLibrary.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"

#include "CLog.h"

UBTDecorator_CheckRandPercent::UBTDecorator_CheckRandPercent()
{
	NodeName = L"CheckRandPercent";
}

bool UBTDecorator_CheckRandPercent::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//float fRandom = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("RandomFloatKey");

	FRandomStream Stream;
	Stream.GenerateNewSeed();
	float fRandom = UKismetMathLibrary::RandomFloatInRangeFromStream(0.000f, 100.000f, Stream);

	CLog::Print(fRandom);
	return fRandom <= Percentage;
}

