#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExistOther.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_ExistOther
	: public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ExistOther();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	
};
