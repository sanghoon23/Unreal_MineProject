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
	/* UpsetCondition 생성 시, 넘어갈 '시작' Delegate */
	FDelStartCondition OnLinkStartUpsetCondition;

	/* UpsetCondition 생성 시, 넘어갈 '종료' Delegate */
	FDelEndCondition OnLinkEndUpsetCondition;

private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* StunConditionUITexture = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Stun ConditionData 에서 사용할 머리 위 표시할 Particle */
		class UParticleSystem* StunHeadParticle; //@Default

	UPROPERTY(EditAnywhere, Category = "Data")
		float StunTime = 0.0f;

	#pragma endregion

public:
	UCDamageType_Stun();

	/* Virtual */
public:
	//@param Subject - 주체자
	//@param DamagedActor - 맞는 액터
	//@param InitialDamageAmount - 초기에 들어갈 데미지 값
	//@param Montage - 맞는 액터가 시행할 Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	/* Function */
public:
	class UTexture2D* GetUITexture() { return StunConditionUITexture; }

	/* Member */
public:
	void SetStunTime(float ApplyStunTime) { StunTime = ApplyStunTime; }
	float GetStunTime() const { return StunTime; }

private:
};
