#include "CDamageType_Poision.h"
#include "Global.h"

UCDamageType_Poision::UCDamageType_Poision()
{
	// Super
	{
		TypeNumber = 0;
		DamageType = FDamageType::POISION;
	}

	// TODO : 
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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/Test_UI_PoisionWithTargetInfo.Test_UI_PoisionWithTargetInfo'";
	ConstructorHelpers::FObjectFinder<UTexture2D> PoisionTexture(*strPath);
	if (PoisionTexture.Succeeded())
	{
		PoisionConditionUITexture = PoisionTexture.Object;
	}
}

void UCDamageType_Poision::OnDamageDelegate(AActor * DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
