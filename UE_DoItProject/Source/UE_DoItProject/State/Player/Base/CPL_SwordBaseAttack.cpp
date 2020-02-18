#include "CPL_SwordBaseAttack.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"

#include "Charactor/Player/CPlayer.h"
#include "_GameController/CPL_TargetingSystem.h"

UCPL_SwordBaseAttack::UCPL_SwordBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCPL_SwordBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set Player
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);

	// Set Delegate "OnActionReset" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bAttackMode = false;
		bAttacking = false;
		bComboCheck = false;
		CurrentComboNum = 0;

		
		bAttackCall = false; //@Attack Call
		bAttackPossible = false; //@AttackPossible
		Player->CanMove(); //@이동가능
		Player->OnGravity(); //@중력키기
	});

	// Set Delegate "Begin Attack" - IIC_BaseAttack
	BeginAttackDeleFunc.AddLambda([&]()
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddUObject(this, &UCPL_SwordBaseAttack::EndAttack);
}


void UCPL_SwordBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// AttackInput 이 들어오면
	if (bAttackCall == true && bAttacking == false)
	{
		// Target 이 있는지 확인
		APawn* FindAttackTarget = Player->GetFindAttackTarget();
		if (FindAttackTarget != nullptr)
		{
			// @AttackRange 보다 크면, Controller Input Vec
			if (FindAttackTarget->GetHorizontalDistanceTo(Player) > AttackRange)
			{
				FVector MoveDir = FindAttackTarget->GetActorLocation() - Player->GetActorLocation();
				MoveDir.Z = 0.0f;
				Player->GetCharacterMovement()->AddInputVector(MoveDir, 1.0f);

				// CLog::Print(L"In MoveCall");
			}
			// @AttackRange 보다 작으면, 공격가능(bAttackPossible), 공격 시작(BegindAttack)
			else
			{
				bAttackPossible = true;
				BeginAttack(Player);

				bAttackCall = false;
			}
		}//(FindAttackTarget != NULL)

		// @회전하면서 공격하면 계속 공격되는 버그.
		// bAttacking == false 로 처리??
		// CanNotMove 로 처리했음. 그냥 그게 제일 자연스러워보임
		APlayerController* controller = GetWorld()->GetFirstPlayerController();
		if (controller != nullptr /* && bAttacking == false */)
		{
			if (controller->IsInputKeyDown(EKeys::W)
				|| controller->IsInputKeyDown(EKeys::S)
				|| controller->IsInputKeyDown(EKeys::A)
				|| controller->IsInputKeyDown(EKeys::D))
			{
				bAttackCall = false;

				return;
			}
		}

	}//(bAttackCall == true)
}


void UCPL_SwordBaseAttack::BeginAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

void UCPL_SwordBaseAttack::EndAttack()
{
	bAttackMode = false;
	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;

	bAttackCall = false;
	bAttackPossible = false;

	Player->CanMove(); //@이동가능
	Player->OnGravity(); //@중력키기
}

void UCPL_SwordBaseAttack::OnComboSet(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);
}

/* TargetSystem 에서 Target 이 있는지 없는지 체크. */
void UCPL_SwordBaseAttack::AttackOtherPawn()
{
}

void UCPL_SwordBaseAttack::ImpulseAttack(float intensity)
{
}

void UCPL_SwordBaseAttack::CheckProcedural()
{
}

