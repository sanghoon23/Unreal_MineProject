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
	bIsFinishing = false;
}

// Task �� ����Ǹ�, BeginAttack �� �����Ű��, EndAttack�� Lamda ����.
// @Warning - �ش� Ŭ���� bIsFinishing �� ���� üũ��.
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
	// BaseAttack->BeginAttack(MonsterPawn);

	// Set Lamda - ���� Ŭ���� bIsFinishing �� �ٷ�.
	bIsFinishing = true;

	// Delegate Handle
	DelegateHandle = BaseAttack->EndAttackDeleFunc.AddLambda([&]() 
	{
		bIsFinishing = false;
	});

	return EBTNodeResult::InProgress; //@InProgress
}

/* MonsterPawn �� BeginAttack �� ��� ���� */
void UBTTask_ComboAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(MonsterPawn);

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MonsterPawn);
	IfNullRet(Charactor);

	// @Combo �� ������ bIsFinishing == false �� ��.
	IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->SetAttackTypeRetIBaseAttack(AttackTypeNum);
	check(BaseAttack);

	// 1. �޺��� ����ƴٸ�,
	if (bIsFinishing == false)
	{
		// ReMove DelegateHandle - (Lambda)
		BaseAttack->EndAttackDeleFunc.Remove(DelegateHandle);

		// Quit Tick Task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	// 2. �ƴ϶��,
	else
	{
		BaseAttack->BeginAttack(MonsterPawn);
	}
}