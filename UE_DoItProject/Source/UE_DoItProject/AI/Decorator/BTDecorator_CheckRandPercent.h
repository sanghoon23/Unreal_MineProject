#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckRandPercent.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_CheckRandPercent
	: public UBTDecorator
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Data")
		/* 0.0f ~ 100.0f ±îÁö */
		float Percentage = 0.0f;

public:
	UBTDecorator_CheckRandPercent();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
