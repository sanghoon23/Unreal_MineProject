#include "CPL_SwordBaseAttack.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "_GameController/CPL_TargetingSystem.h"
#include "System/CS_AttackDecision.h"

UCPL_SwordBaseAttack::UCPL_SwordBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Create System
	AttackDecision = CreateDefaultSubobject<UCS_AttackDecision>("AttackDecision");
}


void UCPL_SwordBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set Player
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);

	#pragma region Set Delegate
	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		//bAttackMode = false;
		//bAttacking = false;
		//bComboCheck = false;
		//CurrentComboNum = 0;
		//bAttackPossible = false; //@AttackPossible

		bInputAttackCall = false; //@Attack Call
		Player->CanMove(); //@이동가능
		Player->OnGravity(); //@중력키기
	});

	//// Set Delegate "Begin Attack" - IIC_BaseAttack
	//BeginAttackDeleFunc.AddLambda([&]()
	//{
	//	bAttacking = true;
	//	bAttackMode = true;
	//});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddUObject(this, &UCPL_SwordBaseAttack::EndAttack);

	#pragma endregion
}


void UCPL_SwordBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//// AttackInput 이 들어오면
	//if (bAutoAttackCall == true && bAttacking == false)
	//{
	//	// Target 이 있는지 확인
	//	APawn* FindAttackTarget = Player->GetFindAttackTarget();
	//	if (FindAttackTarget != nullptr)
	//	{
	//		//// @AttackRange 보다 크면, Controller Input Vec
	//		//if (Player->GetHorizontalDistanceTo(FindAttackTarget) > AttackRange)
	//		//{
	//		//	FVector MoveDir = FindAttackTarget->GetActorLocation() - Player->GetActorLocation();
	//		//	MoveDir.Z = 0.0f;
	//		//	Player->GetCharacterMovement()->AddInputVector(MoveDir, 1.0f);
	//		//}
	//		//// @AttackRange 보다 작으면, 공격가능(bAttackPossible), 공격 시작(BegindAttack)
	//		//else
	//		//{
	//		//	bAutoAttackCall = false;
	//		//	bAttackPossible = true;
	//		//	BeginAttack(Player); //@ 공격 시작
	//		//}
	//	}//(FindAttackTarget != NULL)

	//	// @이동 방향키 누르면, 자동 타겟 공격 취소
	//	APlayerController* controller = Cast<APlayerController>(Player->GetController());
	//	if (controller != nullptr /* && bAttacking == false */)
	//	{
	//		if (controller->IsInputKeyDown(EKeys::W)
	//			|| controller->IsInputKeyDown(EKeys::S)
	//			|| controller->IsInputKeyDown(EKeys::A)
	//			|| controller->IsInputKeyDown(EKeys::D))
	//		{
	//			bAutoAttackCall = false;

	//			return;
	//		}
	//	}
	//}//(bAutoAttackCall == true)

	if (bInputAttackCall == true
		&& AttackDecision->GetAble() == EAutoAttackable::USE
		&& bAttacking == false)
	{
		if (AttackDecision->GetDecisionPossible() == true)
		{
			bInputAttackCall = false;
			bAttackPossible = true;

			BeginAttack(Player);
		}

		// @이동 방향키 누르면, 자동 타겟 공격 취소
		APlayerController* controller = Cast<APlayerController>(Player->GetController());
		if (controller != nullptr /* && bAttacking == false */)
		{
			if (controller->IsInputKeyDown(EKeys::W)
				|| controller->IsInputKeyDown(EKeys::S)
				|| controller->IsInputKeyDown(EKeys::A)
				|| controller->IsInputKeyDown(EKeys::D))
			{
				AttackDecision->StopAttackTrace();

				bAttackPossible = false;
				bInputAttackCall = false;
				return;
			}
		}
	}
}


void UCPL_SwordBaseAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);

	IfNullRet(DoingActor);
	check(DoingActor);

	bInputAttackCall = true;
	if (AttackDecision->GetAble() == EAutoAttackable::USE)
	{
		// Target 이 있는지 확인
		APawn* FindAttackTarget = Player->GetFindAttackTarget();
		if (FindAttackTarget != nullptr)
		{
			AttackDecision->StartAttackTrace(FindAttackTarget);
		}
	}
	else
	{
		bAttackPossible = true;
	}

	////@bAutoAttackCall 이 true 라면, Tick 에서 조건 들어감.
	//(AutoAttackAble == EAutoAttackable::USE)
	//	? bAutoAttackCall = true
	//	: bAttackPossible = true;
}

void UCPL_SwordBaseAttack::EndAttack()
{
	Super::EndAttack();

	//bAttackMode = false;
	//bAttacking = false;
	//bComboCheck = false;
	//CurrentComboNum = 0;

	bInputAttackCall = false;
	bAttackPossible = false;
	Player->CanMove(); //@이동가능
	Player->OnGravity(); //@중력키기
}

void UCPL_SwordBaseAttack::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);

	IfNullRet(DoingActor);
	check(DoingActor);
}

/* TargetSystem 에서 Target 이 있는지 없는지 체크. */
void UCPL_SwordBaseAttack::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_SwordBaseAttack::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_SwordBaseAttack::CheckProcedural()
{
	Super::CheckProcedural();
}

