#include "CDamageType_Freeze.h"
#include "Global.h"

UCDamageType_Freeze::UCDamageType_Freeze()
{
	// Super
	{
		TypeNumber = 0;
		DamageType = FDamageType::FREEZE;
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

	strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/Test_UI_FreezeWithTargetInfo.Test_UI_FreezeWithTargetInfo'";
	ConstructorHelpers::FObjectFinder<UTexture2D> PoisionTexture(*strPath);
	if (PoisionTexture.Succeeded())
	{
		PoisionConditionUITexture = PoisionTexture.Object;
	}
}

void UCDamageType_Freeze::OnDamageDelegate(AActor * DamagedActor)
{
	Super::OnDamageDelegate(DamagedActor);
}
