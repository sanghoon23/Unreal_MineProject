#include "CDamageType_Base.h"

UCDamageType_Base::UCDamageType_Base()
{
	TypeNumber = 0;
	ConditionType = FConditionType::NORMAL;

	bCausedByWorld = false;
	bScaleMomentumByMass = true;
	bRadialDamageVelChange = false;

	DamageImpulse = 100.0f;
	DestructibleImpulse = 100.0f;
	DestructibleDamageSpreadScale = 100.0f;
}

void UCDamageType_Base::OnDamageDelegate(AActor * DamagedActor)
{
	check(DamagedActor);

	// @Delegate ½ÇÇà
	DamageFunc.Broadcast(DamagedActor);
}
