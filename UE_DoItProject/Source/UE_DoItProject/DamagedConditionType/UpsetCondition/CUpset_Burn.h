#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Burn.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCUpset_Burn 
	: public UCBaseConditionType
{
	GENERATED_BODY()
	
private:
	const float DamagedTime = 1.0f;

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
	class UParticleSystemComponent* BurnParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "Data")
	/* �ð� ���� �� ������, */
	float SecondDamage = 0.0f;

	#pragma endregion

public:
	UCUpset_Burn();

	/* Virtual Function */
public:
	/* �ش� Owner �� HitComp AddCondition �� �� ���� �Լ�*/
	//@BaseConditionType.h
	virtual void StartCondition(APawn* Owner) override;

	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ����� �Լ� */
	//@BaseConditionType.h
	virtual void EndCondition(APawn* Owner) override;

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* Function */
public:
	void SetBurnParticleComp(class UParticleSystemComponent* PTComp);

	float GetSecondDamage() const { return SecondDamage; }
	void SetSecondDamage(float fValue) { SecondDamage = fValue; }

	/* Member */
private:
	/* Timer */
	float SecondTimer = 0.0f;
};
