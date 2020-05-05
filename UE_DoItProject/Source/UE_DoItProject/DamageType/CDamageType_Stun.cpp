#include "CDamageType_Stun.h"
#include "Global.h"

UCDamageType_Stun::UCDamageType_Stun()
{
	// Super
	{
		TypeNumber = 0;
		DamageType = FDamageType::STUN;
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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/Test_UI_StunWithTargetInfo.Test_UI_StunWithTargetInfo'";
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
