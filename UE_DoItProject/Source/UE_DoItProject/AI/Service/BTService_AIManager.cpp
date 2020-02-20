#include "BTService_AIManager.h"
#include "Global.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"

UBTService_AIManager::UBTService_AIManager()
{
	NodeName = L"AIManager";
	Interval = 0.1f;
}

// #Edit *0219
// @AI 전체를 관장하는 Manager 를 정의하고,
// AI BehaviorTree 를 잠시 멈출 수 있도록 만들어놓음.
// 예외 상황 처리
void UBTService_AIManager::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 자기 자신 Charactor
	ACharacter* Charactor = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Charactor != nullptr)
	{
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
		check(I_Charactor);

		// @IF TRUE
		// 1. Montage 가 실행중이면, retrun
		if (Charactor->GetMesh()->GetAnimInstance()->Montage_IsPlaying(I_Charactor->GetCurrentApplyedMontage()) == true)
		{
			SetPossibleToBlackboardValue(OwnerComp, false);
			return;
		}

		// @IF FALSE
		{

		}

	}

	// @통과
	SetPossibleToBlackboardValue(OwnerComp, true);
}

void UBTService_AIManager::SetPossibleToBlackboardValue(UBehaviorTreeComponent & OwnerComp, bool bValue)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("bAIRunningPossible", bValue);
}

