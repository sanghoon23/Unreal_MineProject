#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatternAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_PatternAttack 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_PatternAttack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);


};
