#include "CDamageType_Normal.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

UCDamageType_Normal::UCDamageType_Normal()
{
	// Super
	{
		TypeNumber = 0;
		DamageType = FDamageType::NORMAL;
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

void UCDamageType_Normal::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@때린 대상 바라보기
	UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	//@Take Damage
	APawn* DamagedPawn = Cast<APawn>(DamagedActor);
	check(DamagedPawn);
	AController* PawnController = Cast<AController>(Cast<APawn>(DamagedActor));

	FDamageEvent DamageEvent;
	DamageEvent.DamageTypeClass = GetClass();
	DamagedActor->TakeDamage(InitialDamageAmount, DamageEvent, PawnController, DamagedActor);
}

void UCDamageType_Normal::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
