#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ComboAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_ComboAttack 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ComboAttack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	FDelegateHandle DelegateHandle;

	uint8 AttackTypeNum = 0;
	bool bIsFinishing = true;
};
