#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Freeze.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Freeze 
	: public UCDamageType_Base
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* PoisionConditionUITexture = nullptr;

	#pragma endregion

public:
	UCDamageType_Freeze();

	/* Virtual */
public:
	virtual void OnDamageDelegate(AActor* DamagedActor) override;

	/* Function */
public:
	class UTexture2D* GetUITexture() { return PoisionConditionUITexture; }


	/* Member */
public:
	void SetFreezingTime(float fValue) { FreezingTime = fValue; }
	float GetFreezingTime() const { return FreezingTime; }

private:
	float FreezingTime = 0.0f;
	
};
