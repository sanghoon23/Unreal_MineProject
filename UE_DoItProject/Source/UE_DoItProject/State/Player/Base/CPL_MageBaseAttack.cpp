#include "CPL_MageBaseAttack.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "_GameController/CPL_TargetingSystem.h"

UCPL_MageBaseAttack::UCPL_MageBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCPL_MageBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set Player
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);

	// Set Delegate "On Hit" - IIC_Charactor
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
		Player->CanMove(); //@�̵�����
		Player->OnGravity(); //@�߷�Ű��
	});

	// Set Delegate "Begin Attack" - IIC_BaseAttack
	BeginAttackDeleFunc.AddLambda([&]()
	{
		bAttacking = true;
		bAttackMode = true;
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddUObject(this, &UCPL_MageBaseAttack::EndAttack);
}

void UCPL_MageBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// AttackInput �� ������
	if (bAttackCall == true && bAttacking == false)
	{
		// Target �� �ִ��� Ȯ��
		APawn* FindAttackTarget = Player->GetFindAttackTarget();
		if (FindAttackTarget != nullptr)
		{
			// @AttackRange ���� ũ��, Controller Input Vec
			if (FindAttackTarget->GetHorizontalDistanceTo(Player) > AttackRange)
			{
				FVector MoveDir = FindAttackTarget->GetActorLocation() - Player->GetActorLocation();
				MoveDir.Z = 0.0f;
				Player->GetCharacterMovement()->AddInputVector(MoveDir, 1.0f);
			}
			// @AttackRange ���� ������, ���ݰ���(bAttackPossible), ���� ����(BegindAttack)
			else
			{
				bAttackCall = false;
				bAttackPossible = true;
				BeginAttack(Player); //@ ���� ����
			}
		}//(FindAttackTarget != NULL)

		// @�̵� ����Ű ������, �ڵ� Ÿ�� ���� ���
		APlayerController* controller = Cast<APlayerController>(Player->GetController());
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

void UCPL_MageBaseAttack::BeginAttack(AActor * DoingActor)
{
	IfNullRet(DoingActor);
	check(DoingActor);

	// Target �� �ִ��� Ȯ��
	APawn* FindAttackTarget = Player->GetFindAttackTarget();
	if (FindAttackTarget != nullptr)
	{
		// @AttackRange ���� ũ��, Controller Input Vec
		if (FindAttackTarget->GetHorizontalDistanceTo(Player) <= AttackRange)
		{
			bAttackPossible = true;
		}
	}//(FindAttackTarget != NULL)
}

void UCPL_MageBaseAttack::EndAttack()
{
	bAttacking = false;
	bComboCheck = false;
	CurrentComboNum = 0;

	bAttackCall = false;
	bAttackPossible = false;
	Player->CanMove(); //@�̵�����
	Player->OnGravity(); //@�߷�Ű��
}

void UCPL_MageBaseAttack::OnComboSet(AActor * DoingActor)
{
	check(DoingActor);
	IfNullRet(DoingActor);
}

void UCPL_MageBaseAttack::AttackOtherPawn()
{
}

void UCPL_MageBaseAttack::ImpulseAttack(float intensity)
{
}

void UCPL_MageBaseAttack::CheckProcedural()
{
}

