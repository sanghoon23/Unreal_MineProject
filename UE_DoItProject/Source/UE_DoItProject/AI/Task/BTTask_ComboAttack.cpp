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

	CLog::Print(L"I_Charactor NOT NULL!!");

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRetResult(I_AttackComp, EBTNodeResult::Failed);

	CLog::Print(L"I_AttackComp NOT NULL!!");

	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(MonsterPawn->GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

	// BaseAttack
	if (bUsingRange == false)
	{
		CLog::Print(L"SettAttackTypeRetIBaseAttack!!");
		IIC_BaseAttack* BaseAttack = I_AttackComp->SetAttackTypeRetIBaseAttack(AttackTypeNum);
		if (BaseAttack != nullptr)
		{
			BaseAttack->BeginAttack(MonsterPawn);
		}
	}
	else
	{
		//@·£´ý Attack Type - BeginAttack
		//int32 Lower = AttackTypeRange.GetLowerBoundValue();
		//int32 Higher = AttackTypeRange.GetUpperBoundValue();
		int Input = UKismetMathLibrary::RandomIntegerInRange(MinAttackRange, MaxAttackRange);
		CLog::Print(Input);
		IIC_BaseAttack* BaseAttack = I_AttackComp->SetAttackTypeRetIBaseAttack(Input);
		check(BaseAttack);
		if (BaseAttack != nullptr)
		{
			BaseAttack->BeginAttack(MonsterPawn);
		}
	}

	return EBTNodeResult::Succeeded;
}

void UBTTask_ComboAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	//IfNullRet(MonsterPawn);

	//IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(MonsterPawn);
	//IfNullRet(I_Charactor);

	//IIC_AttackComp* I_AttackComp = I_Charactor->GetIAttackComp();
	//IfNullRet(I_AttackComp);

	//IIC_BaseAttack* BaseAttack = I_AttackComp->GetCurrentIBaseAttack();
	//IfNullRet(BaseAttack);

	//UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(MonsterPawn->GetWorld());
	//IfNullRet(Nav);


	//if (BaseAttack->GetAttacking() == true)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
}
