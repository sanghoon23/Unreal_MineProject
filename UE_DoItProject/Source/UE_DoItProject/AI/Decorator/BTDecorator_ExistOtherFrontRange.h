#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_ExistOtherFrontRange.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_ExistOtherFrontRange 
	: public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_ExistOtherFrontRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
