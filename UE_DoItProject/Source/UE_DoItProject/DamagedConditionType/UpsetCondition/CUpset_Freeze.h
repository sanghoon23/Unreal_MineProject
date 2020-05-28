#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Freeze.generated.h"

/*
@Warning -
�� ���´� ������ FreezeParticleComp �� �����Ѵٴ� �������� ����
*/

UCLASS()
class UE_DOITPROJECT_API UCUpset_Freeze 
	: public UCBaseConditionType
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* FreezeParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* FreezeParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* FreezeUnderParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* FreezeUnderParticleComp;

	UPROPERTY(VisibleAnywhere, Category = "DestructibleActor")
		class ACDM_FreezingBroken* DM_FreezenBroken;

	#pragma endregion

public:
	UCUpset_Freeze();

	/* Virtual Function */
public:
	/* �ش� Owner �� HitComp AddCondition �� �� ���� �Լ�*/
	//@BaseConditionType.h
	virtual void StartCondition(APawn* Owner) override;

	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ����� �Լ� */
	//@BaseConditionType.h
	virtual void EndCondition(APawn * Owner) override;

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;
	
	/* Function */
public:
	void SetFreezeParticle(class UParticleSystem* PT);
	void SetFreezeUnderParticle(class UParticleSystem* PT);

};
