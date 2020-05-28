#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Stun.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCUpset_Stun 
	: public UCBaseConditionType
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* NonActionMon = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystem* StunHeadParticle;

	UPROPERTY(VisibleAnywhere, Category = "Particle")
		class UParticleSystemComponent* StunHeadParticleComp;

	#pragma endregion

public:
	UCUpset_Stun();

	/* Virtual Function */
public:
	/* �ش� Owner �� HitComp AddCondition �� �� ���� �Լ�*/
//@BaseConditionType.h
	virtual void StartCondition(APawn* Owner) override;

	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ����� �Լ� */
	virtual void EndCondition(APawn * Owner) override;

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* Function */
public:
	void SetStunHeadPrticle(class UParticleSystem* PT);

	/* Member */
public:
	void SetMontage(class UAnimMontage* Montage);

};
