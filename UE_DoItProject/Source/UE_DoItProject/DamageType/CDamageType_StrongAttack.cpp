#include "CDamageType_StrongAttack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

UCDamageType_StrongAttack::UCDamageType_StrongAttack()
{
	// Super
	{
		TypeNumber = 3;
		DamageType = FDamageType::STRONGATTACK;
	}

	// Attribute
	{
		bCausedByWorld = false;
		bScaleMomentumByMass = true;
		bRadialDamageVelChange = false;

		DamageImpulse = 100.0f;
		DestructibleImpulse = 100.0f;
		DestructibleDamageSpreadScale = 100.0f;
	}
}

void UCDamageType_StrongAttack::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@때린 대상 바라보기
	UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	//@Take Damage
	APawn* DamagedPawn = Cast<APawn>(DamagedActor);
	if (DamagedPawn != nullptr)
	{
		AController* PawnController = Cast<AController>(Cast<APawn>(DamagedActor));

		FDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = GetClass();
		DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, DamagedActor);
	}
}

void UCDamageType_StrongAttack::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}

