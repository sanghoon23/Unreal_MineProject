#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LookAtTarget.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_LookAtTarget
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_LookAtTarget();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
