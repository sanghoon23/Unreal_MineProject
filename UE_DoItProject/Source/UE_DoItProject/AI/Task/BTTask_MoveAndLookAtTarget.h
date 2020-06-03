#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveAndLookAtTarget.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_MoveAndLookAtTarget 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveAndLookAtTarget();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
