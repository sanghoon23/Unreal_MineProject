#include "BTTask_PatternAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "AIController.h"

#include "AI/Controller/Base/CAIC_BaseHM.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

UBTTask_PatternAttack::UBTTask_PatternAttack()
{
	NodeName = L"PatternAttack";

	bNotifyTick = true; // Tick True;
}

EBTNodeResult::Type UBTTask_PatternAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* MonsterPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(MonsterPawn, EBTNodeResult::Failed);

	ACAIC_BaseHM* BaseHMController = Cast<ACAIC_BaseHM>(OwnerComp.GetAIOwner());
	IfNullRetResult(BaseHMController, EBTNodeResult::Failed);

	int TypeNum = BaseHMController->GetTypePatternAttackState(); //@Pattern
	if (TypeNum == -1)
	{
		return  EBTNodeResult::Failed;
	}

	IIC_Charactor* Charactor = Cast<IIC_Charactor>(MonsterPawn);
	IfNullRetResult(Charactor, EBTNodeResult::Failed);

	IIC_AttackComp* I_AttackComp = Charactor->GetIAttackComp();
	IfNullRetResult(I_AttackComp, EBTNodeResult::Failed);

	IIC_BaseAttack* CurrentIBaseAttack = I_AttackComp->GetCurrentIBaseAttack();
	IfNullRetResult(CurrentIBaseAttack, EBTNodeResult::Failed);
	CurrentIBaseAttack->EndAttackDeleFunc.Broadcast();

	IIC_BaseAttack* I_BaseAttack 
		= I_AttackComp->SetAttackTypeRetIBaseAttack(TypeNum);
	IfNullRetResult(I_BaseAttack, EBTNodeResult::Failed);
	I_BaseAttack->BeginAttack(MonsterPawn);

	return EBTNodeResult::Succeeded;
}

void UBTTask_PatternAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
