#include "CHM_ShamanEnergyPa.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Monster/CHM_Shaman.h"

#include "Actor/Projectile/CProjectile_MagicBall.h"
#include "Notify/Event/CN_SpawnProjectile.h"

UCHM_ShamanEnergyPa::UCHM_ShamanEnergyPa()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Super Setting
	{
		CurrentComboNum = 0;
		MaxComboNum = 1;
	}

	FString Path = L"";

	////@LOAD Particle
	//{
	//	Path = L"";
	//	ConstructorHelpers::FObjectFinder<UParticleSystem> P_LightingCast(*Path);
	//	if (P_LightingCast.Succeeded())
	//	{
	//		ParticleLightingCast = P_LightingCast.Object;
	//	}
	//}

#pragma region FirstAttack

	// Setting
	{
		UAnimMontage* Shaman_FirstAttack = nullptr;

		Path = L"AnimMontage'/Game/_Mine/Montages/HM_Shaman/Attack/HM_Shaman_Mon_EnergyPa.HM_Shaman_Mon_EnergyPa'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> MFirstAttack(*Path);
		if (MFirstAttack.Succeeded())
			Shaman_FirstAttack = MFirstAttack.Object;

		AttackMontages.Emplace(Shaman_FirstAttack);
	}

#pragma endregion

}

void UCHM_ShamanEnergyPa::BeginPlay()
{
	Super::BeginPlay();

	// @Set HM_Basic
	HM_Shaman = Cast<ACHM_Shaman>(GetOwner());
	check(HM_Shaman);

#pragma region DamageType

	DT_Normal = NewObject<UCDamageType_Normal>();
	DT_Normal->SetDamageImpulse(10.0f);

#pragma endregion

	//@Notify Reference
	TArray<FAnimNotifyEventReference> NotifyEvent_Mon_0;
	AttackMontages[0]->GetAnimNotifies(0, 15.0f, false, NotifyEvent_Mon_0);
	for (auto& Ref : NotifyEvent_Mon_0) //@AttackMontage[0]
	{
		const FAnimNotifyEvent* Event = Ref.GetNotify();
		UCN_SpawnProjectile* Notify = Cast<UCN_SpawnProjectile>(Event->Notify);
		if (Notify != nullptr)
		{
			Notifies_SpawnProjectile.Add(Notify);
		}
	}

}

void UCHM_ShamanEnergyPa::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	///* 이 공격이 실행된다면 IsRunTick 에 의해서 Tick 이 호출되므로 항상 bAttacking == true */
	//if (bAttacking == true)
	//{
	//	FName CurrentSectionName = HM_Shaman->GetMesh()->GetAnimInstance()->Montage_GetCurrentSection(AttackMontages[0]);
	//	if (CurrentSectionName == SectionStartName)
	//	{
	//	}
	//	else
	//	{
	//		//@On Particle Object
	//		AActor* TargetInAI = HM_Shaman->GetTargetInAI();
	//		if (TargetInAI != nullptr)
	//		{
	//		}
	//	}
	//}
}

void UCHM_ShamanEnergyPa::IsRunTick(bool bRunning)
{
	SetComponentTickEnabled(bRunning);
}

void UCHM_ShamanEnergyPa::BeginAttack(AActor * DoingActor)
{
	Super::BeginAttack(DoingActor);
	check(DoingActor);

	ACHM_Shaman* Pawn = Cast<ACHM_Shaman>(DoingActor);
	check(Pawn);

	// @IF TRUE RETURN
	IfTrueRet(Pawn->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Pawn->GetIEquipComp()->GetEquiping()); //@Equping Check
	IfTrueRet(IsLastCombo());

	//@Notify Ref - Setting Target
	AActor* TargetInAI = HM_Shaman->GetTargetInAI();
	for (auto& Notify : Notifies_SpawnProjectile)
	{
		Notify->SpeedValue = 800.0f;
		if (TargetInAI != nullptr)
		{
			Notify->SetProjectileDirection(TargetInAI->GetActorLocation() - Pawn->GetActorLocation());
			Notify->SetProjectileTarget(TargetInAI);
		}
		else
		{
			Notify->SetProjectileDirection(Pawn->GetActorForwardVector());
		}
	}

	if (bAttacking == false)
	{
		Pawn->ActorAnimMonPlay
		(
			AttackMontages[0], /* @FirstMontage == Combo1 */
			0.9f, false
		);
	}
}



