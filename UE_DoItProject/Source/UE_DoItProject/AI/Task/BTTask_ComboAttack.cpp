#include "BTTask_ComboAttack.h"
#include "Global.h"
#include "NavigationSystem.h"
#include "AIController.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

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

	CLog::Print(L"AI BeginAttack!!");

	return EBTNodeResult::InProgress;
}

/* MonsterPawn 의 BeginAttack 을 계속 실행 */
void UBTTask_ComboAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(MonsterPawn);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MonsterPawn);
	IfNullRet(Charactor);

	// @Combo 가 끝나면 bIsFinishing == false 가 됨.
	IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->GetCurrentIBaseAttack();
	check(BaseAttack);

	CLog::Print(L"In Task!!");

	bool bAttacking = BaseAttack->GetAttacking();
	if (bAttacking == false)
	{
		CLog::Print(L"Combo End!!");

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum
		(
			"AIState", 0 //@NONE
		);

		// Quit Tick Task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
