#include "BTTask_ComboAttack.h"
#include "Global.h"
#include "NavigationSystem.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "AI/Controller/CAIC_HM_Basic.h"

UBTTask_ComboAttack::UBTTask_ComboAttack()
{
	NodeName = L"ComboAtatck";

	bNotifyTick = true; // Tick True;
}

EBTNodeResult::Type UBTTask_ComboAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(MonsterPawn, EBTNodeResult::Failed);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MonsterPawn);
	IfNullRetResult(Charactor, EBTNodeResult::Failed);

	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(MonsterPawn->GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

	// BaseAttack
	IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->SetAttackTypeRetIBaseAttack(AttackTypeNum);
	BaseAttack->BeginAttack(MonsterPawn);

	return EBTNodeResult::InProgress;
}

/* MonsterPawn 의 BeginAttack 을 계속 실행 */
void UBTTask_ComboAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(MonsterPawn);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MonsterPawn);
	IfNullRet(Charactor);

	// @Combo 가 끝나면 bIsFinishing == false 가 됨.
	IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->GetCurrentIBaseAttack();
	check(BaseAttack);

	bool bAttacking = BaseAttack->GetAttacking();

	// 1. 콤보가 종료됐다면,
	if (bAttacking == false)
	{
		//CLog::Print(L"Combo End!!");

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", static_cast<uint8>(EAIState_Basic::NONE));

		// Quit Tick Task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
