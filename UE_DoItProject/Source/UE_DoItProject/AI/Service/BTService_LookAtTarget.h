#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookAtTarget.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTService_LookAtTarget 
	: public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_LookAtTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
