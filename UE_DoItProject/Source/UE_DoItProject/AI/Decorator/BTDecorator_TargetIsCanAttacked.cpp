#include "BTDecorator_TargetIsCanAttacked.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "AIController.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

UBTDecorator_TargetIsCanAttacked::UBTDecorator_TargetIsCanAttacked()
{
	NodeName = L"TargetIsCanAttacked";
}

bool UBTDecorator_TargetIsCanAttacked::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 자기 자신 Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, false);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IIC_Charactor* I_TargetCharactor = Cast<IIC_Charactor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(I_TargetCharactor, false);

	IIC_HitComp* I_HitComp = I_TargetCharactor->GetIHitComp();
	IfNullRetResult(I_TargetCharactor, false);

	return I_HitComp->IsCanAttackedFromOther(); //@Player 무적 시,
}

