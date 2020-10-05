#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_RandomAttack 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomAttack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
