#include "BTDecorator_IsBattle.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_BaseAttack.h"

UBTDecorator_IsBattle::UBTDecorator_IsBattle()
{
	NodeName = L"IsBattle";
}

bool UBTDecorator_IsBattle::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, false);

	//@AI 의 Target 이 공격 중인지 판별
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Target);
	if (I_Charactor != nullptr)
	{
		bool bAttacking = I_Charactor->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking();
		bool bBeating = I_Charactor->GetIHitComp()->IsBeated();
		if (bAttacking == true && bBeating == true)
		{
			Result = true;
		}
	}
	else return false;

	//IfTrueRet(Player->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking()); //@Attacking

	return Result;
}
