#include "BTDecorator_ExistOtherAttackLine.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "AIController.h"

#include "Interface/IC_Monster.h"

UBTDecorator_ExistOtherAttackLine::UBTDecorator_ExistOtherAttackLine()
{
	NodeName = L"ExistOtherAttackLine";
}

bool UBTDecorator_ExistOtherAttackLine::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	// 자기 자신 Pawn
	APawn* ControlPawn = OwnerComp.GetAIOwner()->GetPawn();
	IfNullRetResult(ControlPawn, false);

	// Target
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("Target"));
	IfNullRetResult(Target, false);

	// World
	UWorld* World = ControlPawn->GetWorld();
	IfNullRetResult(World, false);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector Dir = Target->GetActorLocation() - ControlPawn->GetActorLocation();
	Dir.Normalize();
	FVector Start = ControlPawn->GetActorLocation();
	FVector End = Start + (Dir * 10000.0f);

	TArray<AActor *> Ignore;
	Ignore.Add(ControlPawn);

	EDrawDebugTrace::Type Debug = EDrawDebugTrace::ForOneFrame;

	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), Start, End, 
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3), //@MonsterAttack
		true, Ignore, Debug, HitResult, true
	);

	//@
	if (bHit == true)
	{
		if (HitResult.GetActor() == Target)
		{
			return true;
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsEnum
			(
				"AIState", 0 //@NONE
			);
			return false;
		}
	}

	return false;
}
