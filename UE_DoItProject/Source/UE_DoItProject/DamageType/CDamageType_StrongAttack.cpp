#include "CDamageType_StrongAttack.h"

UCDamageType_StrongAttack::UCDamageType_StrongAttack()
{
	// Super
	{
		TypeNumber = 0;
		ConditionType = FConditionType::STRONGATTACK;
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

void UCDamageType_StrongAttack::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}

