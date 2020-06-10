#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ExistOtherAttackLine.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_ExistOtherAttackLine 
	: public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_ExistOtherAttackLine();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
