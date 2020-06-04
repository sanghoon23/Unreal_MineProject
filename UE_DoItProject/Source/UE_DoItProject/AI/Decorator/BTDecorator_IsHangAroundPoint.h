#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHangAroundPoint.generated.h"

UCLASS()
class UE_DOITPROJECT_API UBTDecorator_IsHangAroundPoint
	: public UBTDecorator
{
	GENERATED_BODY()
	
/*
HangAround(발견시 이리저리 이동할 범위) 에
Charactor 가 왔는지 판별,
맞다면 이리저리 이동,
아니라면 Target 으로 이동하는걸로
*/

public:
	UBTDecorator_IsHangAroundPoint();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
