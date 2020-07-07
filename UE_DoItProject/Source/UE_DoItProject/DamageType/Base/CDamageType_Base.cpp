#include "CDamageType_Base.h"
#include "Global.h"

#include "Component/Base/C_BaseHitComp.h"

UCDamageType_Base::UCDamageType_Base()
{
	TypeNumber = 0;
	DamageType = FDamageType::NORMAL;

	bCausedByWorld = false;
	bScaleMomentumByMass = true;
	bRadialDamageVelChange = false;

	DamageImpulse = 0.0f;
	DestructibleImpulse = 100.0f;
	DestructibleDamageSpreadScale = 100.0f;
}

void UCDamageType_Base::OnHittingProcess(AActor* Subject, AActor* DamagedActor, UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount)
{
	check(Subject);
	check(DamagedActor);
	check(DamagedActorHitComp);
}

void UCDamageType_Base::SetDamageImpulse(float DamageAmount)
{
	DamageImpulse = DamageAmount;
}

void UCDamageType_Base::SetDestructibleImpulse(float DestructibleDamageAmount)
{
	DestructibleImpulse = DestructibleDamageAmount;
}

void UCDamageType_Base::SetDestructibleDamageSpreadScale(float SpreadScale)
{
	DestructibleDamageSpreadScale = SpreadScale;
}
