#include "CDamageType_Normal.h"

UCDamageType_Normal::UCDamageType_Normal()
{
	// Super
	{
		TypeNumber = 0;
		ConditionType = FConditionType::NORMAL;
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

void UCDamageType_Normal::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
