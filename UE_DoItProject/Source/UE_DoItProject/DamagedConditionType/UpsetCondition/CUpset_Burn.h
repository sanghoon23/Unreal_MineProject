#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Burn.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCUpset_Burn 
	: public UCBaseConditionType
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
	class UParticleSystemComponent* BurnParticleComp;

	#pragma endregion

public:
	UCUpset_Burn();

	/* Virtual Function */
public:
	/* �ش� Owner �� HitComp AddCondition �� �� ���� �Լ�*/
	//@BaseConditionType.h
	virtual void StartConditionOnActor(AActor* Owner) override;

	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateConditionOnActor(AActor* Owner, float DeltaTime) override;

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ����� �Լ� */
	//@BaseConditionType.h
	virtual void EndConditionOnActor(AActor * Owner) override;

	/* Function */
public:
	void SetBurnParticleComp(class UParticleSystemComponent* PTComp) { BurnParticleComp = PTComp; }

	float GetSecondDamage() const { return OneSecondDamage; }
	void SetSecondDamage(float fValue) { OneSecondDamage = fValue; }

	/* Member */
private:
	/* �ʴ� ������, */
	float OneSecondDamage = 0.0f;
};
