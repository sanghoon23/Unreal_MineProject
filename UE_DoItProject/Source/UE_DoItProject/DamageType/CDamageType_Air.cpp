#include "CDamageType_Air.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"

#include "Interface/IC_Charactor.h"

UCDamageType_Air::UCDamageType_Air()
{
	// Super
	{
		TypeNumber = 1;
		DamageType = FDamageType::AIR;
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

void UCDamageType_Air::OnHittingProcess(AActor * Subject, AActor * DamagedActor, UC_BaseHitComp * DamagedActorHitComp, float InitialDamageAmount)
{
	Super::OnHittingProcess(Subject, DamagedActor, DamagedActorHitComp, InitialDamageAmount);

	//@때린 대상 바라보기
	UCFL_ActorAgainst::LookAtTarget(DamagedActor, Subject);

	ACharacter* Charactor = Cast<ACharacter>(DamagedActor);
	if (Charactor != nullptr)
	{
		IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(DamagedActor);
		check(I_Charactor);

		FVector Location = DamagedActor->GetActorLocation();
		Location.Z += 200.0f;
		DamagedActor->SetActorLocation(Location);

		// @속력 줄이기 - 중력끄고 바로 해줘야함
		Charactor->GetCharacterMovement()->Velocity = FVector(0.0f);

		// @중력 끄기.
		I_Charactor->OffGravity();
	}

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

void UCDamageType_Air::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
