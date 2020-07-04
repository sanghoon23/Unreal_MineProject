#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "Interface/IC_MeshParticle.h"

#include "CUpset_Slower.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCUpset_Slower 
	: public UCBaseConditionType
{
	GENERATED_BODY()

private:
	const float DamagedTime = 1.0f;

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
		/* Burn ConditionData ���� ����� Particle */
		TMap<EAttachPointType, class UParticleSystem*> SlowerParticlesMap;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		TArray<class UParticleSystemComponent*> PTCompContainerForEnd;

	//UPROPERTY(VisibleAnywhere, Category = "Data")
	//	/* �ð� ���� �� ������, */
	//	float SecondDamage = 0.0f;

#pragma endregion
	
public:
	UCUpset_Slower();

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
	void SetSlowerParticle(TMap<EAttachPointType, class UParticleSystem*>& In);

	/* Member */
private:
	/* Timer */
	float SecondTimer = 0.0f;

};
