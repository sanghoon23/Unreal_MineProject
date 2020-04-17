#include "CPL_ActionPullActorWithCable.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/Cable/CPL_CableObject.h"

UCPL_ActionPullActorWithCable::UCPL_ActionPullActorWithCable()
{
	PrimaryComponentTick.bCanEverTick = true;
	//..

	FString Path = L"";

	#pragma region Set Montage
	// Set Ready Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/CommonMon_PullingActor.CommonMon_PullingActor'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> pullReadyMon(*Path);
		if (pullReadyMon.Succeeded())
			PullReadyMontage = pullReadyMon.Object;
	}

	#pragma endregion

	#pragma region DamageType

	DT_Stun = NewObject<UCDamageType_Stun>();
	DT_Stun->SetStunTime(10.0f);

	#pragma endregion
}

void UCPL_ActionPullActorWithCable::BeginPlay()
{
	Super::BeginPlay();
	
	//@Get Player Pointer
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_ActionPullActorWithCable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_ActionPullActorWithCable::OnAction()
{
	IIC_BaseAttack* BaseAttack = Player->GetIAttackComp()->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		IfTrueRet(BaseAttack->GetAttacking()); // @IF TRUE RETURN
	}

	// @IF NULL RETURN
	APawn* Target = Player->GetFindAttackTarget();
	IfNullRet(Target); //@Target 이 설정되지 않았으면, return;

	// @대상과의 거리가 너무 멀 시,
	float DistanceToTarget = Player->GetDistanceTo(Target);
	IfTrueRet(DistanceToTarget > UsingActionRange);

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@IsFalling
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @몽타주 실행
	Player->ActorAnimMonPlay(PullReadyMontage, 1.0f, true);
	Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection
	(
		FName("Streching"), PullReadyMontage
	);
}

void UCPL_ActionPullActorWithCable::BeginActionState()
{
	Super::BeginActionState();

	//@Get CableObject Pointer
	CableObject = Player->GetCableObject();
	check(CableObject);

	//@Get Taget
	AActor* Target = Player->GetFindAttackTarget();
	check(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@'Action' Block
	Player->OnBlockAction();

	//@대상 바라보기
	UCFL_ActorAgainst::LookAtTarget(Target, Player);

	//@초기 LerpValue 설정 - PullRange 의 비율
	float InitDistance = Player->GetDistanceTo(Target);
	LerpValue = (InitDistance - PullRange) / PullRange;

	//@타겟 세팅
	CableObject->SetPullStartTarget(Target);
}

void UCPL_ActionPullActorWithCable::TickActionState()
{
	Super::TickActionState();

	//@Get CableObject Pointer
	CableObject = Player->GetCableObject();
	check(CableObject);

	//@Get Taget
	AActor* Target = Player->GetFindAttackTarget();
	check(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//@끌어 당겨질 때,
	IfFalseRet(CableObject->GetPulling());
	if (bNextMontage == false)
	{
		//@JumpToSection
		Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection
		(
			FName("Pulling"), PullReadyMontage
		);

		//@I_Charactor
		IIC_Charactor* Charactor = Cast<IIC_Charactor>(Target);
		if (Charactor != nullptr)
		{
			//@I_HitCompo
			IIC_HitComp* HitComp = Charactor->GetIHitComp();
			if (HitComp != nullptr)
			{
				// 1.1 Set Hit Attribute
				HitComp->SetHitDirection(FVector(0.0f));

				// 1.2 OnHit
				HitComp->SetHitMoveSpeed(0.0f);
				HitComp->OnHit(Player, DT_Stun, 5.0f);
			}
		}

		bNextMontage = true;
	}//@bNextMontage

	//@당겨지는 중, TargetLocation Setting
	PullingTargetLocation(Target);
}

void UCPL_ActionPullActorWithCable::EndActionState()
{
	Super::EndActionState();

	//@This Value
	bNextMontage = false;

	//@CableObject Reset
	CableObject->ResetState();

/////////////////////////////////////////////////////////////////////////////////

	//@Release - 'Action' Block
	Player->OffBlockAction();
}

/* Target 을 끌어오는 동안 위치 조정 */
// @Warning - BeginAction 구한 값인 LerpValue 사용.
void UCPL_ActionPullActorWithCable::PullingTargetLocation(AActor * Target)
{
	//@Target Location
	FVector TargetLocation = Target->GetActorLocation();

	//@Player Location + PullRange
	FVector PlayerLocation = Player->GetActorLocation();
	PulledDirection = PlayerLocation - TargetLocation;
	PulledDirection.Normalize();
	float Distance = Player->GetDistanceTo(Target);
	if (Distance > PullRange)
	{
		//@Calc
		TargetLocation += PulledDirection * PulledSpeed * LerpValue;

		//@Set Location
		Target->SetActorLocation(TargetLocation);
	}
}
