#include "CPL_MageBaseAttack.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "System/CS_AttackDecision.h"

UCPL_MageBaseAttack::UCPL_MageBaseAttack()
{
	PrimaryComponentTick.bCanEverTick = true;

	//Create System
	AttackDecision = CreateDefaultSubobject<UCS_AttackDecision>("AttackDecision");
}


void UCPL_MageBaseAttack::BeginPlay()
{
	Super::BeginPlay();

	// @Set Player
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	// Set Delegate "On Hit" - IIC_Charactor
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(IC_Charactor);
	IC_Charactor->OnActionResetState.AddLambda([&](AActor*)
	{
		bInputAttackCall = false; //@Attack Call
	});

	// Set Delegate "End Attack" - IIC_BaseAttack
	EndAttackDeleFunc.AddUObject(this, &UCPL_MageBaseAttack::EndAttack);
}

void UCPL_MageBaseAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

		// @�̵� ����Ű ������, �ڵ� Ÿ�� ���� ���
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
	}//(bInputAttackCall == true)
}

void UCPL_MageBaseAttack::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);

	IfNullRet(DoingActor);
	check(DoingActor);

	bInputAttackCall = true;
	if (AttackDecision->GetAble() == EAutoAttackable::USE)
	{
		// Target �� �ִ��� Ȯ��
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
}

void UCPL_MageBaseAttack::EndAttack()
{
	Super::EndAttack();

	bInputAttackCall = false;
	Player->CanMove(); //@�̵�����
	Player->OnGravity(); //@�߷�Ű��
}

void UCPL_MageBaseAttack::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);

	check(DoingActor);
	IfNullRet(DoingActor);
}

void UCPL_MageBaseAttack::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_MageBaseAttack::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_MageBaseAttack::CheckProcedural()
{
	Super::CheckProcedural();
}

