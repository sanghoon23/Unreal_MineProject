#include "CPL_MageBaseAttack.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_BaseAttack.h"
#include "Charactor/Player/CPlayer.h"
#include "System/CS_AttackDecision.h"

#include "UI/HUD_Main.h"

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
		// Target 이 있는지 확인
		APawn* FindAttackTarget = Player->GetFindAttackTarget();
		if (FindAttackTarget != nullptr)
		{
			AttackDecision->StartAttackTrace(FindAttackTarget);
		}
		else
		{
			//@UI
			check(MainHUD);
			FString Input = L"Tab 을 눌러 타겟을 지정하세요!!";
			MainHUD->VisibleUITextNotify(Input, 3.0f);
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
	Player->CanMove(); //@이동가능
	Player->OnGravity(); //@중력키기
}

void UCPL_MageBaseAttack::OnComboSet(AActor * DoingActor)
{
	Super::OnComboSet(DoingActor);

	check(DoingActor);
	IfNullRet(DoingActor);
}

void UCPL_MageBaseAttack::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
}

void UCPL_MageBaseAttack::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_MageBaseAttack::CheckProcedural()
{
	Super::CheckProcedural();
}

