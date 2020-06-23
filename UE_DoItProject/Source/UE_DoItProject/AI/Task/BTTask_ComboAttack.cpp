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
	if (bUsingRange == false)
	{
		IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->SetAttackTypeRetIBaseAttack(AttackTypeNum);
		check(BaseAttack);
		BaseAttack->BeginAttack(MonsterPawn);
	}
	else
	{
		//@·£´ý Attack Type - BeginAttack
		//int32 Lower = AttackTypeRange.GetLowerBoundValue();
		//int32 Higher = AttackTypeRange.GetUpperBoundValue();
		int Input = UKismetMathLibrary::RandomIntegerInRange(MinAttackRange, MaxAttackRange);
		CLog::Print(Input);
		IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->SetAttackTypeRetIBaseAttack(Input);
		check(BaseAttack);
		BaseAttack->BeginAttack(MonsterPawn);
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_ComboAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	//IfNullRet(MonsterPawn);

	//IIC_Charactor* Charactor = Cast<IIC_Charactor>(MonsterPawn);
	//IfNullRet(Charactor);

	//// @Combo °¡ ³¡³ª¸é bIsFinishing == false °¡ µÊ.
	//IIC_BaseAttack* BaseAttack = Charactor->GetIAttackComp()->GetCurrentIBaseAttack();
	//check(BaseAttack);

	//bool bAttacking = BaseAttack->GetAttacking();
	//if (bAttacking == false)
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsEnum
	//	(
	//		"AIState", 0 //@NONE
	//	);


	//	// Quit Tick Task
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
}
