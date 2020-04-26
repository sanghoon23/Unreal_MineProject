#include "CDamageType_Stun.h"
#include "Global.h"

UCDamageType_Stun::UCDamageType_Stun()
{
	// Super
	{
		TypeNumber = 0;
		ConditionType = FConditionType::STUN;
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

	FString strPath = L"";

	strPath = L"Texture2D'/Engine/EngineResources/Bad.Bad'";
	ConstructorHelpers::FObjectFinder<UTexture2D> StunTexture(*strPath);
	if (StunTexture.Succeeded())
	{
		StunConditionUITexture = StunTexture.Object;
	}
}

void UCDamageType_Stun::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
