#include "BTService_HMBasic.h"
#include "Global.h"

#include "Interface/IC_Monster.h"
#include "Interface/IC_Charactor.h"

#include "AIController.h"

UBTService_HMBasic::UBTService_HMBasic()
{
	NodeName = L"Service_HMBasic";
	Interval = 0.02f;
}

void UBTService_HMBasic::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 자기 자신 Pawn
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRet(OwnerPawn);

	EAIState_Basic CurrentAIState = static_cast<EAIState_Basic>(OwnerComp.GetBlackboardComponent()->GetValueAsEnum("AIState"));
	if (CurrentAIState == EAIState_Basic::NONE)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", static_cast<uint8>(EAIState_Basic::HANGAROUND));
	}
	else if (CurrentAIState == EAIState_Basic::HANGAROUND)
	{
		Timer += DeltaSeconds;
		if (Timer > EndHangAroundTime)
		{
			Timer = 0.0f;
			//OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", static_cast<uint8>(EAIBehaviorState::CANATTACK));

			//Test Code
			//@AI 의 Target 이 공격 중인지 판별
			IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(OwnerPawn);
			if (I_Charactor != nullptr)
			{
				//bool bAttacking = I_Charactor->GetIAttackComp()->GetCurrentIBaseAttack()->GetAttacking();
				bool bBeating = I_Charactor->GetIHitComp()->IsBeated();
				if (bBeating == false)
				{
					//CLog::Print(L"bBeating == false In !!");
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum("AIState", static_cast<uint8>(EAIState_Basic::CANATTACK));
				}
			}
		}
	}
	else if (CurrentAIState == EAIState_Basic::CANATTACK)
	{
	}

}
