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
	/* UpsetCondition ���� ��, �Ѿ '����' Delegate */
	FDelStartCondition OnLinkStartUpsetCondition;

	/* UpsetCondition ���� ��, �Ѿ '����' Delegate */
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
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
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
