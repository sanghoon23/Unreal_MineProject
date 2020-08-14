#include "CPL_SDAttackWindmilSlash.h"
#include "Global.h"

#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "System/CS_AttackDecision.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_SwordAttackComp.h"

UCPL_SDAttackWindmilSlash::UCPL_SDAttackWindmilSlash()
{
	PrimaryComponentTick.bCanEverTick = false;

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
}

void UCPL_SDAttackWindmilSlash::BeginPlay()
{
	Super::BeginPlay();
	
	#pragma region Super

	//@"UNABLE" - Auto AttackDecision System
	AttackDecision->UnAble();

	#pragma endregion

	//Set Delegate
	EndAttackDeleFunc.AddUObject(this, &UCPL_SDAttackWindmilSlash::EndAttack);

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
		IfFalseRet(GetAttackPossible()); // @Super::Tick 에서 처리 중.
	}

	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo()); //@IsLastCombo

////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Before
	//APawn* Target = Player->GetFindAttackTarget();
	//if (Target == nullptr)
	//{
	//	EndAttackDeleFunc.Broadcast();
	//	return;
	//}
	//check(Target);
	//// @타겟 바라보게 하기
	//UCFL_ActorAgainst::LookAtTarget(Player, Target);

	// @ON Block Key
	Player->OnBlockKeyInput();

	if (bAttacking == false)
	{
		Player->ActorAnimMonPlay
		(
			SwordAttackMontages[0], /* @FirstMontage == Combo1 */
			1.0f, true				// @AnimPlay 무조건 실행.
		);
	}
}

void UCPL_SDAttackWindmilSlash::EndAttack()
{
	Super::EndAttack();

	// @OFF Block Key
	Player->OffBlockKeyInput();
}

void UCPL_SDAttackWindmilSlash::AttackOtherPawn(UCDamageType_Base* DamageType)
{
	Super::AttackOtherPawn(DamageType);
	check(DamageType);

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Position = Player->GetActorLocation();

	FCollisionShape sphere = FCollisionShape::MakeSphere(AttackRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	float DebugLifeTime = 1.0f;
	TArray<FOverlapResult> OverlapResults;
	bool bOverlap = GetWorld()->OverlapMultiByChannel //@Single - 단일.
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
					UE_LOG(LogTemp, Warning, L"SDAttackWindmilSlash CallAttack - HitComp Null!!");
			}
			else
				UE_LOG(LogTemp, Warning, L"SDAttackWindmilSlash CallAttack - Charactor Null!!");
		}
	}

}

void UCPL_SDAttackWindmilSlash::ImpulseAttack(float intensity)
{
	Super::ImpulseAttack(intensity);
}

void UCPL_SDAttackWindmilSlash::CheckProcedural()
{
	Super::CheckProcedural();
}

