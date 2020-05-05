#include "CDamageType_AirAttack.h"

UCDamageType_AirAttack::UCDamageType_AirAttack()
{
	// Super
	{
		TypeNumber = 0;
		DamageType = FDamageType::AIRATTACK;
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

void UCDamageType_AirAttack::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}


