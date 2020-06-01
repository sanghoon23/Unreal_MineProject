#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsBattle.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_IsBattle 
	: public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsBattle();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	
};
