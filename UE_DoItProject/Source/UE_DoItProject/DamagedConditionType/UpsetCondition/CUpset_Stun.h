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
	/* 해당 Owner 의 HitComp AddCondition 될 때 실행 함수*/
//@BaseConditionType.h
	virtual void StartCondition(APawn* Owner) override;

	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행될 함수 */
	virtual void EndCondition(APawn * Owner) override;

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* Function */
public:
	void SetStunHeadPrticle(class UParticleSystem* PT);

	/* Member */
public:
	void SetMontage(class UAnimMontage* Montage);

};
