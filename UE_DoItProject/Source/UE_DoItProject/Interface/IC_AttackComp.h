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
	virtual void AttackCall() {}

	// @Type - �ٲ� AttackType �� �Ѱ���.
	// ��, Type �� Setting �԰� ���ÿ� 
	// �׿� �ش��ϴ� Interface_BaseAttack �� �Ѱ���.
	virtual IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) = 0;

	// @������ AttackType ���� Interface �� ������
	// @Warning 
	// ���ϵ� IC_BaseAttack �� ���� ���̰ų� �� ������ ���� �� ����.
	// �׷��� �ʱ� ���� SetAttackTypeRetIBaseAttack �� �����.
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() = 0;

public:
	// virtual IIC_BaseAttack* GetAttackStateMember(uint8 Num) { return nullptr; }
};

