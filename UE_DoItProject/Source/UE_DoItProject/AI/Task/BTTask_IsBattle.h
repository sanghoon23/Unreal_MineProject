#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IsBattle.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_IsBattle 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_IsBattle();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
