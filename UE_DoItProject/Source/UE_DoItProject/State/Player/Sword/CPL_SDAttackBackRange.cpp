#include "CPL_SDAttackBackRange.h"
#include "Global.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"

#include "UI/HUD_Main.h"

UCPL_SDAttackBackRange::UCPL_SDAttackBackRange()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Super
	/* Super */
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;

		AttackRange = 400.0f;
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
}

void UCPL_SDAttackBackRange::BeginPlay()
{
	Super::BeginPlay();

	//@Running Tick
	IsRunTick(false);

	#pragma region Super

	//@Auto AttackDecision System
	AttackDecision->OnAble(Player, AttackRange);

	#pragma endregion

	//Set Delegate
	EndAttackDeleFunc.AddUObject(this, &UCPL_SDAttackBackRange::EndAttack);

}

void UCPL_SDAttackBackRange::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_SDAttackBackRange::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
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
	if (Target == nullptr)
	{
		EndAttackDeleFunc.Broadcast();
		return;
	}
	check(Target);

	// @타겟 바라보게 하기
	UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @ON BLOCK KEY INPUT
	Player->OnBlockKeyInput();

	float StartTime = 0.0f;
	float EndSectionTime = 0.0f;
	SwordAttackMontages[0]->GetSectionStartAndEndTime(1, StartTime, EndSectionTime);

	//@Delegate And SetTimer
	EndSectionDelegate.BindUFunction(this, FName("GoBackToTarget"), Target);
	GetWorld()->GetTimerManager().SetTimer(EndSectionTimerHandle, EndSectionDelegate, 1.0f, false, EndSectionTime);

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0],
			1.0f, true				// @AnimPlay 무조건 실행.
		);
	}
	//Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Start"), SwordAttackMontages[0]);
}

void UCPL_SDAttackBackRange::EndAttack()
{
	Super::EndAttack();

	// @OFF Block Key
	Player->OffBlockKeyInput();
}

void UCPL_SDAttackBackRange::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Position = Player->GetActorLocation();

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	float DebugLifeTime = 1.0f;
	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel
	(
		OverlapResults
		, Position
		, FQuat::Identity
		, ECC_GameTraceChannel2 // @PlayerAttack
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), Position, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif // ENABLE_DRAW_DEBUG

	if (bOverlap == true)
	{
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			IIC_Charactor* Charactor = Cast<IIC_Charactor>(OverlapResult.GetActor());
			if (Charactor != nullptr)
			{
				// 1. Get Interface HitComp
				IIC_HitComp* HitComp = Charactor->GetIHitComp();
				if (HitComp != nullptr)
				{
					// 1.1 Set Hit Attribute
					FVector PlayerLocation = Player->GetActorLocation();
					FVector HitDirection = OverlapResult.GetActor()->GetActorLocation() - PlayerLocation;
					HitDirection.Normalize();
					HitDirection.Z = 0.0f;
					HitComp->SetHitDirection(HitDirection);
					HitComp->SetHitMoveSpeed(DamageType->GetHitMoveSpeed());

					// 1.2 Hit Delegate - Air(DamageType)
					HitComp->OnHit(Player, DamageType, DamageType->DamageImpulse);
				}
				else
					UE_LOG(LogTemp, Warning, L"SDAttackBackRange CallAttack - HitComp Null!!");
			}
			else
				UE_LOG(LogTemp, Warning, L"SDAttackBackRange CallAttack - Charactor Null!!");
		}
	}

}

void UCPL_SDAttackBackRange::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_SDAttackBackRange::CheckProcedural()
{
	Super::CheckProcedural();
}

void UCPL_SDAttackBackRange::GoBackToTarget(AActor * Target)
{
	Player->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("NextAction"), SwordAttackMontages[0]);

	FVector PlayerLocation = Player->GetActorLocation();
	FVector TargetLoaction = Target->GetActorLocation();
	FVector Direction = TargetLoaction - PlayerLocation;
	Direction.Normalize();

	//@Player 위치 설정 후 다시,
	Player->SetActorLocation(TargetLoaction + (Direction * 200.0f));

	//@타겟 바라보기
	UCFL_ActorAgainst::LookAtTarget(Player, Target);
}
