#include "CPL_SDAttackBackRange.h"
#include "Global.h"

#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"

UCPL_SDAttackBackRange::UCPL_SDAttackBackRange()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Super
	/* Super */
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}
	#pragma endregion

	FString Path = L"";

	#pragma region SwordAttackMontage
	// SwordAttackMontage
	{
		UAnimMontage* Sword_BackRangeAttack_1 = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_BackRangeAttack.SwordMon_BackRangeAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MBackRangeAttack(*Path);
		if (MBackRangeAttack.Succeeded())
			Sword_BackRangeAttack_1 = MBackRangeAttack.Object;

		SwordAttackMontages.Emplace(Sword_BackRangeAttack_1);
	}
	#pragma endregion

	#pragma region Create DamageType

	DT_Strong = NewObject<UCDamageType_StrongAttack>();

	#pragma endregion

}

void UCPL_SDAttackBackRange::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion
}

void UCPL_SDAttackBackRange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackBackRange::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick 에서 처리 중.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

	// @타겟 바라보게 하기
	LookAtTarget(Target);

	// @공격 중 조금씩 이동 - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay 무조건 실행.
		);
	}
}

void UCPL_SDAttackBackRange::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_SDAttackBackRange::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_SDAttackBackRange::CheckProcedural()
{
	Super::CheckProcedural();
}

void UCPL_SDAttackBackRange::LookAtTarget(AActor * Target)
{
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}
