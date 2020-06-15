#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetState.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTTask_SetState 
	: public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AISetting")
		FName AIStateEntryName = "";

	UPROPERTY(EditAnywhere, Category = "AISetting")
		uint8 EnumValue = 0;

public:
	UBTTask_SetState();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
