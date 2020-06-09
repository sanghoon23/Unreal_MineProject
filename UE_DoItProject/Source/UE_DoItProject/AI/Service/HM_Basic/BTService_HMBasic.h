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

	/* Member */
private:
	float Timer = 0.0f;
	float EndHangAroundTime = 10.0f;
};
