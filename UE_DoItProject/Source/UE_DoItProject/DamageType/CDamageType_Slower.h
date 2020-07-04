#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "Interface/IC_MeshParticle.h"
#include "Ability/Base/CBaseAbility.h"

#include "CDamageType_Slower.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Slower 
	: public UCDamageType_Base
{
	GENERATED_BODY()

#pragma region Reflection

private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* SlowerConditionUITexture = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData ���� ����� Particle */
		TMap<EAttachPointType, class UParticleSystem*> SlowerParticlesMap;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UCBaseAbility* AbilitySlower;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		float SecondDamageValue = 0.0f;


#pragma endregion

public:
	UCDamageType_Slower();

	/* Virtual */
public:
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	virtual void OnDamageDelegate(AActor* DamagedActor) override;
	
	/* Function */
public:
	class UTexture2D* GetUITexture() { return SlowerConditionUITexture; }

	/* Member */
public:
	void SetSlowerTime(float ApplySlowerTime) { SlowerTime = ApplySlowerTime; }
	float GetSlowerTime() const { return SlowerTime; }

	void SetSlowerSpeedValue(float ApplySlowerValue) { SlowerValue = ApplySlowerValue; }
	float GetSlowerSpeedValue() const { return SlowerValue; }

	/* BaseAbility �߰�. Slower */
	void SetAbilityForSlower(class UCBaseAbility* Ability);

	/* @Slower �� ���� �ٸ� Particle �� ���� �� ���� */
	void AddSlowerParticle(EAttachPointType Type, class UParticleSystem* PT);

public:
	float SlowerTime = 0.0f;

	float SlowerValue = 0.0f;
};
