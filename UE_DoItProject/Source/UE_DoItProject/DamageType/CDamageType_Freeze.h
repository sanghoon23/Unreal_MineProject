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
		/* Freeze ConditionData ���� ����� Particle */
		class UParticleSystem* FreezeParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Freeze ConditionData ���� ����� Freeze Under Particle */
		class UParticleSystem* FreezeUnderParticle;

	#pragma endregion

public:
	UCDamageType_Freeze();

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
	class UTexture2D* GetUITexture() { return FreezeConditionUITexture; }


	/* Member */
public:
	void SetFreezingTime(float fValue) { FreezingTime = fValue; }
	float GetFreezingTime() const { return FreezingTime; }

private:
	float FreezingTime = 0.0f;
	
};
