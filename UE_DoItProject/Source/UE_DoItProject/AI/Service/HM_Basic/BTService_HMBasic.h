#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_HMBasic.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTService_HMBasic 
	: public UBTService
{
	GENERATED_BODY()

public:
	UBTService_HMBasic();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
