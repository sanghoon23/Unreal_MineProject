#include "CPlayerAnimInst.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_EquipComp.h"

UCPlayerAnimInst::UCPlayerAnimInst()
{
}

void UCPlayerAnimInst::NativeBeginPlay()
{
	Player = Cast<ACPlayer>(TryGetPawnOwner());
	IfNullRet(Player);

	Charactor = Cast<IIC_Charactor>(Player);
	IfNullRet(Charactor);

	UActorComponent* ActorComp = Player->GetComponentByClass(UCInverseKinematics::StaticClass());
	if (ActorComp != nullptr)
	{
		FootIK = Cast<UCInverseKinematics>(ActorComp);
	}

	//OnMontageBlendingOut.AddDynamic(this, &UCPlayerAnimInst::Test);
}

void UCPlayerAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	IfNullRet(Player);
	// @Move
	Direction = CalculateDirection(Player->GetVelocity(), Player->GetActorRotation());
	Speed = Player->GetVelocity().Size();

	// @FindFloorDistance - Jump 동작 자연스럽게 하기 위해서
	{
		// *Case 1
		//FVector CapsuleLocation = Player->GetActorLocation();
		//FFindFloorResult FloorResult;
		//Player->GetCharacterMovement()->FindFloor(CapsuleLocation, FloorResult, true);
		//FindFloorDistance = FloorResult.GetDistanceToFloor();
		// FloorResult.GetDistanceToFloor();

		// *Case 2
		// FindFloorDistance = FootTraceDistance();
		// CLog::Print(FindFloorDistance);
	}

	// @Jumping
	bCharactorJumping = Player->IsJumping();

	// @bInAir - 공중에 떠있으면서, Gravity 가 0.0f 이 아닐 때,
	if (Player->GetCharacterMovement()->IsFalling()
		&& Player->GetCharacterMovement()->GravityScale > 0.0f
		/*&& Player->IsJumping() == true*/)
	{
		bInAir = true;
	}
	else
	{
		bInAir = false;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////

	IfNullRet(Charactor);
	CurrentStateType = Player->GetCurrentAttackStateType();
	bArmed = Charactor->GetIEquipComp()->GetArmed();

	//IfNullRet(BaseAttack);
	//bAttackMode = BaseAttack->GetAttackMode();

	IfNullRet(FootIK);
	FeetEffector = FootIK->GetFeetIKEffector();
	HandEffector = FootIK->GetHandIKEffector();
}

/*  
#Edit 0306 - Jump 동작
Jump 동작을 자연스럽게 하려했는데,
InAir 체크로 판단하는게 맞았음;;
*/
float UCPlayerAnimInst::FootTraceDistance()
{
	FVector start = Player->GetActorLocation();

	float CapsuleHalfHeight = Player->GetDefaultHalfHeight();
	float TraceDistance = 400.0f;

	float z = start.Z - CapsuleHalfHeight - TraceDistance;
	FVector end = FVector(start.X, start.Y, z);

	TArray<AActor *> ignore;
	ignore.Add(Player);

	FHitResult hit;
	EDrawDebugTrace::Type debug = EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, ignore, debug, hit, true
	);

	if (hit.IsValidBlockingHit())
	{
		float length = (hit.ImpactPoint - hit.TraceEnd).Size();
		return length;
	}

	return 0.0f;
}

//void UCPlayerAnimInst::Test(UAnimMontage * Montage, bool bInterrupted)
//{
//	//CLog::Print(Montage->GetName());
//	//CLog::Print(bInterrupted);
//}
