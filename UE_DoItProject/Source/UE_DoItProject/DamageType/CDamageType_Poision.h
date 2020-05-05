#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Poision.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Poision 
	: public UCDamageType_Base
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* PoisionConditionUITexture = nullptr;

	#pragma endregion

public:
	UCDamageType_Poision();

	/* Virtual */
public:
	virtual void OnDamageDelegate(AActor* DamagedActor) override;

	/* Function */
public:
	class UTexture2D* GetUITexture() { return PoisionConditionUITexture; }


	/* Member */
public:
	void SetPoisioningTime(float fValue) { PoisioningTime = fValue; }
	float GetPoisioningTime() const { return PoisioningTime; }

private:
	float PoisioningTime = 0.0f;
};
