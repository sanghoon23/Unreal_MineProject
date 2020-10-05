#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CalcRandomFloatKey.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_CalcRandomFloatKey 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_CalcRandomFloatKey();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
