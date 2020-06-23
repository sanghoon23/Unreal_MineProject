#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ComboAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_ComboAttack 
	: public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "AIControl")
		uint8 AttackTypeNum = 0;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		bool bUsingRange = false;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		int32 MinAttackRange;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		int32 MaxAttackRange;

public:
	UBTTask_ComboAttack();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
