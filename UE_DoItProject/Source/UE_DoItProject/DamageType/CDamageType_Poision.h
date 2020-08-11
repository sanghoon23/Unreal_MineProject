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
public:
	/* UpsetCondition 생성 시, 넘어갈 '시작' Delegate */
	FDelStartCondition OnLinkStartUpsetCondition;

	/* UpsetCondition 생성 시, 넘어갈 '종료' Delegate */
	FDelEndCondition OnLinkEndUpsetCondition;

private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* PoisionConditionUITexture = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		float SecondDamageValue = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Data")
		float PoisioningTime = 0.0f;

	#pragma endregion

public:
	UCDamageType_Poision();

	/* Virtual */
public:
	//@param Subject - 주체자
	//@param DamagedActor - 맞는 액터
	//@param InitialDamageAmount - 초기에 들어갈 데미지 값
	//@param Montage - 맞는 액터가 시행할 Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	/* Function */
public:
	class UTexture2D* GetUITexture() { return PoisionConditionUITexture; }


	/* Member */
public:
	void SetPoisioningTime(float fValue) { PoisioningTime = fValue; }
	float GetPoisioningTime() const { return PoisioningTime; }

	void SetSecondDamageValue(float ApplyDamage) { SecondDamageValue = ApplyDamage; }
	float GetSecondDamageValue() const { return SecondDamageValue; }

private:
};
