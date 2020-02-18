#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TurnToTarget.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_TurnToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TurnToTarget();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
