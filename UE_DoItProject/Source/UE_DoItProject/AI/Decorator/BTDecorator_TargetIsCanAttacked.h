#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetIsCanAttacked.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_TargetIsCanAttacked
	: public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetIsCanAttacked();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
