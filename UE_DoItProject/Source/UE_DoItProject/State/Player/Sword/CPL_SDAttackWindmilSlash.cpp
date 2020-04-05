#include "CPL_SDAttackWindmilSlash.h"
#include "Global.h"

#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"

UCPL_SDAttackWindmilSlash::UCPL_SDAttackWindmilSlash()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Super
	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}
	#pragma endregion

	FString Path = L"";

	#pragma region SwordAttackMontage
	// SwordAttackMontage
	{
		UAnimMontage* Sword_WindmilSlashAttack_1 = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Sword/MAttack/SwordMon_WindmilSlashAttack.SwordMon_WindmilSlashAttack'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MSword_WindmilSlashAttack_1(*Path);
		if (MSword_WindmilSlashAttack_1.Succeeded())
			Sword_WindmilSlashAttack_1 = MSword_WindmilSlashAttack_1.Object;

		SwordAttackMontages.Emplace(Sword_WindmilSlashAttack_1);
	}
	#pragma endregion

	#pragma region Create DamageType

	DT_Air = NewObject<UCDamageType_Air>();

	#pragma endregion

}

void UCPL_SDAttackWindmilSlash::BeginPlay()
{
	Super::BeginPlay();
	
	#pragma region Super

	//@"UNABLE" - Auto AttackDecision System
	AttackDecision->UnAble();

	#pragma endregion

}

void UCPL_SDAttackWindmilSlash::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackWindmilSlash::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	// Super
	{
		IfFalseRet(GetAttackPossible()); // @Super::Tick ���� ó�� ��.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	APawn* Target = Player->GetFindAttackTarget();
	check(Target);

	// @Ÿ�� �ٶ󺸰� �ϱ�
	LookAtTarget(Target);

	// @���� �� ���ݾ� �̵� - AttackMoveDir(I_BaseAttack Value)
	AttackMoveDir = Player->GetActorForwardVector();
	AttackMoveSpeed = 2.0f;

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay ������ ����.
		);
	}
}

void UCPL_SDAttackWindmilSlash::AttackOtherPawn()
{
	Super::AttackOtherPawn();
}

void UCPL_SDAttackWindmilSlash::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_SDAttackWindmilSlash::CheckProcedural()
{
	Super::CheckProcedural();
}

void UCPL_SDAttackWindmilSlash::LookAtTarget(AActor * Target)
{
	check(Target);
	FVector DestVec = Target->GetActorLocation() - Player->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Player->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}

