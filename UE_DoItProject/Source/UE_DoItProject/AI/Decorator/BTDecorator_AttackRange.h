#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackRange.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_AttackRange 
	: public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_AttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
