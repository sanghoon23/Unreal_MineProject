#include "CDamageType_Burn.h"
#include "Global.h"

UCDamageType_Burn::UCDamageType_Burn()
{
	// Super
	{
		TypeNumber = 0;
		DamageType = FDamageType::BURN;
	}

	// Attribute
	{
		bCausedByWorld = false;
		bScaleMomentumByMass = true;
		bRadialDamageVelChange = false;

		DamageImpulse = 5.0f;
		DestructibleImpulse = 0.0f;
		DestructibleDamageSpreadScale = 100.0f;
	}

	FString strPath = L"";

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/Test_UI_BurnWithTargetInfo.Test_UI_BurnWithTargetInfo'";
	ConstructorHelpers::FObjectFinder<UTexture2D> BurnTexture(*strPath);
	if (BurnTexture.Succeeded())
	{
		BurnConditionUITexture = BurnTexture.Object;
	}
}

void UCDamageType_Burn::OnDamageDelegate(AActor* DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}

