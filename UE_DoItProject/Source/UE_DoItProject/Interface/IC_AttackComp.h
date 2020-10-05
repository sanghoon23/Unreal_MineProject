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
	// @Type - �ٲ� AttackType �� �Ѱ���.
	// ��, Type �� Setting �԰� ���ÿ� 
	// �׿� �ش��ϴ� Interface_BaseAttack �� �Ѱ���.
	virtual IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) = 0;

	// @������ AttackType ���� Interface �� ������
	// @Warning 
	// ���ϵ� IC_BaseAttack �� ���� ���̰ų� �� ������ ���� �� ����.
	// ���� �ٲٰ� �ʹٸ�, SetAttackTypeRetIBaseAttack �� �����.
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() = 0;

	/* @�ش� Charactor �� AttackComponent ���� IBaseAttack ArrayData �� ������ */
	//@param -
	virtual void GetBaseAttackList(TArray<class UC_BaseAttackState*>& Out) = 0;

	virtual class UC_BaseAttackState* GetBaseAttack(uint8 Type) = 0;
};

