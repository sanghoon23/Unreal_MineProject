#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckHomeRange.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_CheckHomeRange 
	: public UBTDecorator
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Range")
		float RangeDistance = 0.0f;

public:
	UBTDecorator_CheckHomeRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;


};
