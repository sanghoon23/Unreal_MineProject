#include "BTService_HMBasic.h"
#include "Global.h"

#include "Interface/IC_Monster.h"
#include "Interface/IC_Charactor.h"

UBTService_HMBasic::UBTService_HMBasic()
{
	NodeName = L"Service_HMBasic";
	Interval = 0.02f;
}

void UBTService_HMBasic::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

}
