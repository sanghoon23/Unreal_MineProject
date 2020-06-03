#include "BTTask_IsBattle.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_BaseAttack.h"

#include "AI/Controller/CAIC_HM_Basic.h"
#include "NavigationSystem.h"

UBTTask_IsBattle::UBTTask_IsBattle()
{
	// Name
	NodeName = L"IsBattle";

	// @Tick
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_IsBattle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	//@AI 의 Target 이 공격 중인지 판별
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(OwnerComp.GetAIOwner());
	if (I_Charactor != nullptr)
	{
		bool bAttacking = I_Charactor->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking();
		bool bBeating = I_Charactor->GetIHitComp()->IsBeated();
		if (bAttacking == true && bBeating == true)
		{
			return EBTNodeResult::Failed;
		}
	}

	return EBTNodeResult::Succeeded;
}

