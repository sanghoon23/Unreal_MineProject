#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AIManager.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTService_AIManager : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_AIManager();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void SetPossibleToBlackboardValue(UBehaviorTreeComponent & OwnerComp, bool bValue);
};
