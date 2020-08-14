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
public:
	/* UpsetCondition ���� ��, �Ѿ '����' Delegate */
	FDelStartCondition OnLinkStartUpsetCondition;

	/* UpsetCondition ���� ��, �Ѿ '����' Delegate */
	FDelEndCondition OnLinkEndUpsetCondition;


private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* BurnConditionUITexture = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData ���� ����� Particle */
		class UParticleSystem* BurnParticle;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		float SecondDamageValue = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		float BurnTime = 0.0f;

	#pragma endregion

public:
	UCDamageType_Burn();

	/* Virtual */
public:
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	virtual void SettingData(const FDamageData& Data) override;

	/* Function */
public:
	class UTexture2D* GetUITexture() { return BurnConditionUITexture; }

	/* Member */
public:
	void SetBurnTime(float ApplyBurnTime) { BurnTime = ApplyBurnTime; }
	float GetBurnTime() const { return BurnTime; }

	void SetSecondDamageValue(float ApplyDamage) { SecondDamageValue = ApplyDamage; }
	float GetSecondDamageValue() const { return SecondDamageValue; }

private:
};
