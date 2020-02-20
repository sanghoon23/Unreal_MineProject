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
// @AI ��ü�� �����ϴ� Manager �� �����ϰ�,
// AI BehaviorTree �� ��� ���� �� �ֵ��� ��������.
// ���� ��Ȳ ó��
void UBTService_AIManager::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// �ڱ� �ڽ� Charactor
	ACharacter* Charactor = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Charactor != nullptr)
	{
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
		check(I_Charactor);

		// @IF TRUE
		// 1. Montage �� �������̸�, retrun
		if (Charactor->GetMesh()->GetAnimInstance()->Montage_IsPlaying(I_Charactor->GetCurrentApplyedMontage()) == true)
		{
			SetPossibleToBlackboardValue(OwnerComp, false);
			return;
		}

		// @IF FALSE
		{

		}

	}

	// @���
	SetPossibleToBlackboardValue(OwnerComp, true);
}

void UBTService_AIManager::SetPossibleToBlackboardValue(UBehaviorTreeComponent & OwnerComp, bool bValue)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool("bAIRunningPossible", bValue);
}

