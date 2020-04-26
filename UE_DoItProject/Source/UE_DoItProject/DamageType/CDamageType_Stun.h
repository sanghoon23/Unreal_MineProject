#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Stun.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Stun : public UCDamageType_Base
{
	GENERATED_BODY()

	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Texture")
		class UTexture2D* StunConditionUITexture = nullptr;

private:


	#pragma endregion

public:
	UCDamageType_Stun();

	/* Virtual */
public:
	virtual void OnDamageDelegate(AActor* DamagedActor) override;

	/* Function */
public:


	/* Member */
public:
	void SetStunTime(float ApplyStunTime) { StunTime = ApplyStunTime; }
	float GetStunTime() const { return StunTime; }

private:
	float StunTime = 0.0f;
};
