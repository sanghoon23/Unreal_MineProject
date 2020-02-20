#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_BaseAttack.h"

#include "IC_AttackComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_AttackComp : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_AttackComp
{
	GENERATED_BODY()

	/* Pure Function */
public:
	// @Type - 바꿀 AttackType 을 넘겨줌.
	// 즉, Type 을 Setting 함과 동시에 
	// 그에 해당하는 Interface_BaseAttack 을 넘겨줌.
	virtual IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) = 0;

	// @설정된 AttackType 값의 Interface 를 가져옴
	// @Warning 
	// 리턴된 IC_BaseAttack 은 현재 값이거나 그 이전의 값일 수 있음.
	// 값을 바꾸고 싶다면, SetAttackTypeRetIBaseAttack 을 써야함.
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() = 0;
};

