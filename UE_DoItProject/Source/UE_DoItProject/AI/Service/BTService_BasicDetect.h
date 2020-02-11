#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BasicDetect.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTService_BasicDetect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_BasicDetect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
