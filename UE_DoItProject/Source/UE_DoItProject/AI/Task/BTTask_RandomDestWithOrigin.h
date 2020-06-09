#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomDestWithOrigin.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_RandomDestWithOrigin 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomDestWithOrigin();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
