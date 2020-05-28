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
		class UTexture2D* FreezeConditionUITexture = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData 에서 사용할 Particle */
		class UParticleSystem* FreezeParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData 에서 사용할 Freeze Under Particle */
		class UParticleSystem* FreezeUnderParticle;

	#pragma endregion

public:
	UCDamageType_Freeze();

	/* Virtual */
public:
	//@param Subject - 주체자
	//@param DamagedActor - 맞는 액터
	//@param InitialDamageAmount - 초기에 들어갈 데미지 값
	//@param Montage - 맞는 액터가 시행할 Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	virtual void OnDamageDelegate(AActor* DamagedActor) override;

	/* Function */
public:
	class UTexture2D* GetUITexture() { return FreezeConditionUITexture; }


	/* Member */
public:
	void SetFreezingTime(float fValue) { FreezingTime = fValue; }
	float GetFreezingTime() const { return FreezingTime; }

private:
	float FreezingTime = 0.0f;
	
};
