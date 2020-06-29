#include "CPL_ActionPullActorWithCable.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/Cable/CPL_CableObject.h"

#include "UI/HUD_Main.h"

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
	DT_Stun->SetStunTime(10.0f); //@Stun 시간 지정

	#pragma endregion
}

void UCPL_ActionPullActorWithCable::BeginPlay()
{
	Super::BeginPlay();
	
	//@Get Player Pointer
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

#pragma region UI
	//@UI
	PlayerController = Cast<APlayerController>(Player->GetController());
	check(PlayerController);
	if (PlayerController != nullptr)
	{
		MainHUD = Cast<AHUD_Main>(PlayerController->GetHUD());
		check(MainHUD);
	}

#pragma endregion

#pragma region Set Delegate
	//@Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);

	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		//@Target Init
		//Target = nullptr;

		//@Montage
		bNextMontage = false;

		////@CableObject Reset
		//CableObject->ResetState();
	});
#pragma endregion

}

void UCPL_ActionPullActorWithCable::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_ActionPullActorWithCable::OnAction()
{
	Super::OnAction();

	IIC_BaseAttack* BaseAttack = Player->GetIAttackComp()->GetCurrentIBaseAttack();
	if (BaseAttack != nullptr)
	{
		IfTrueRet(BaseAttack->GetAttacking()); // @IF TRUE RETURN
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@IsFalling
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @IF NULL RETURN
	Target = Player->GetFindAttackTarget();
	if (Target == nullptr) //@Target 이 설정되지 않았으면, return;
	{
		//@UI
		check(MainHUD);
		MainHUD->VisibleUITextNotify
		(
			FString(L"Tab 을 눌러 타겟을 지정하세요!!"),
			3.0f
		);
		return;
	}

	// @대상과의 거리가 너무 멀 시,
	float DistanceToTarget = Player->GetDistanceTo(Target);
	if (DistanceToTarget > UsingActionRange)
	{
		//@UI
		check(MainHUD);
		MainHUD->VisibleUITextNotify
		(
			FString(L"대상과의 거리가 멉니다.."),
			3.0f
		);
		return;
	}

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

// @Warning - Target 을 변수로 저장하고 있음. - Finish Attack 과 동일
// 이 동작은 도중에 Target을 ESC 해서 nullptr 이 되버려도 실행되게끔 함.
void UCPL_ActionPullActorWithCable::BeginActionState()
{
	Super::BeginActionState();

	//@Get CableObject Pointer
	CableObject = Player->GetCableObject();
	check(CableObject);

	CableObject->ResetState();

	////@Get Taget
	//AActor* Target = Player->GetFindAttackTarget();
	//check(Target);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@'Action' Block
	Player->OnBlockAction();

	//@대상 바라보기
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

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

	////@Get Taget
	//AActor* Target = Player->GetFindAttackTarget();
	//check(Target);

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

	//@ Target Init
	Target = nullptr;

	//@Montage
	bNextMontage = false;

	//@CableObject Reset
	CableObject->ResetState();

/////////////////////////////////////////////////////////////////////////////////

	//@Release - 'Action' Block
	Player->OffBlockAction();
}

/* Target 을 끌어오는 동안 위치 조정 */
// @Warning - BeginAction 구한 값인 LerpValue 사용.
void UCPL_ActionPullActorWithCable::PullingTargetLocation(AActor * PulledTarget)
{
	check(PulledTarget);

	//@Target Location
	FVector TargetLocation = PulledTarget->GetActorLocation();

	//@Player Location + PullRange
	FVector PlayerLocation = Player->GetActorLocation();
	PulledDirection = PlayerLocation - TargetLocation;
	PulledDirection.Normalize();
	float Distance = Player->GetDistanceTo(PulledTarget);
	if (Distance > PullRange)
	{
		//@Calc
		TargetLocation += PulledDirection * PulledSpeed * LerpValue;

		//@Set Location
		PulledTarget->SetActorLocation(TargetLocation);
	}
}
