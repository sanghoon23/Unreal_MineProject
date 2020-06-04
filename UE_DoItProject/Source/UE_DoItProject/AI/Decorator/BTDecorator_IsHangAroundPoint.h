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
HangAround(�߽߰� �̸����� �̵��� ����) ��
Charactor �� �Դ��� �Ǻ�,
�´ٸ� �̸����� �̵�,
�ƴ϶�� Target ���� �̵��ϴ°ɷ�
*/

public:
	UBTDecorator_IsHangAroundPoint();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
