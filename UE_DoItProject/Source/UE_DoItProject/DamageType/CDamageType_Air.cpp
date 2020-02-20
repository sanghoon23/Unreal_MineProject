#include "CDamageType_Air.h"

UCDamageType_Air::UCDamageType_Air()
{
	// Super
	{
		TypeNumber = 0;
		ConditionType = FConditionType::AIR;
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

void UCDamageType_Air::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
