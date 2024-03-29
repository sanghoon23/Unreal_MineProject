#include "BTTask_ComboAttack.h"
#include "Global.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

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

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRetResult(I_AttackComp, EBTNodeResult::Failed);

	UNavigationSystemV1* Nav = UNavigationSystemV1::GetNavigationSystem(MonsterPawn->GetWorld());
	IfNullRetResult(Nav, EBTNodeResult::Failed);

	//@순서대로 공격하기
	//@MinAttackRange / MaxAttackRange 사용
	{
		if (bUsingInOrder == true)
		{
			IIC_BaseAttack* BaseAttack = I_AttackComp->SetAttackTypeRetIBaseAttack(StartingOrderAttackType);
			check(BaseAttack);
			if (BaseAttack != nullptr)
			{
				BaseAttack->BeginAttack(MonsterPawn);
			}

			++StartingOrderAttackType;
			StartingOrderAttackType %= MaxAttackRange + 1;
			if(StartingOrderAttackType == 0) StartingOrderAttackType += MinAttackRange;

			return EBTNodeResult::Succeeded;
		}
	}

	// BaseAttack
	if (bUsingRange == false)
	{
		IIC_BaseAttack* BaseAttack = I_AttackComp->SetAttackTypeRetIBaseAttack(AttackTypeNum);
		if (BaseAttack != nullptr)
		{
			BaseAttack->BeginAttack(MonsterPawn);
		}
	}
	else
	{
		//@랜덤 Attack Type - BeginAttack
		int Input = UKismetMathLibrary::RandomIntegerInRange(MinAttackRange, MaxAttackRange);
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
