#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotateExistOther.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_RotateExistOther 
	: public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_RotateExistOther();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	
};
