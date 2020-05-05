#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Burn.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Burn 
	: public UCDamageType_Base
{
	GENERATED_BODY()
	
	#pragma region Reflection

private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* BurnConditionUITexture = nullptr;

	#pragma endregion

public:
	UCDamageType_Burn();

	/* Virtual */
public:
	virtual void OnDamageDelegate(AActor* DamagedActor) override;

	/* Function */
public:
	class UTexture2D* GetUITexture() { return BurnConditionUITexture; }

	/* Member */
public:
	void SetBurnTime(float ApplyStunTime) { BurnTime = ApplyStunTime; }
	float GetBurnTime() const { return BurnTime; }

private:
	float BurnTime = 0.0f;
};
