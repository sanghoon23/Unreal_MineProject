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
	/* UpsetCondition ���� ��, �Ѿ '����' Delegate */
	FDelStartCondition OnLinkStartUpsetCondition;

	/* UpsetCondition ���� ��, �Ѿ '����' Delegate */
	FDelEndCondition OnLinkEndUpsetCondition;

private:
	UPROPERTY(VisibleAnywhere, Category = "Texture")
		class UTexture2D* StunConditionUITexture = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Stun ConditionData ���� ����� �Ӹ� �� ǥ���� Particle */
		class UParticleSystem* StunHeadParticle; //@Default

	UPROPERTY(EditAnywhere, Category = "Data")
		float StunTime = 0.0f;

	#pragma endregion

public:
	UCDamageType_Stun();

	/* Virtual */
public:
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
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
