#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrol.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_FindPatrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPatrol();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
