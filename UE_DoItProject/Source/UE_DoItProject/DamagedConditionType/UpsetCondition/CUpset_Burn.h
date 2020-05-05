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
	/* 해당 Owner 의 HitComp AddCondition 될 때 실행 함수*/
	//@BaseConditionType.h
	virtual void StartConditionOnActor(AActor* Owner) override;

	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateConditionOnActor(AActor* Owner, float DeltaTime) override;

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행될 함수 */
	//@BaseConditionType.h
	virtual void EndConditionOnActor(AActor * Owner) override;

	/* Function */
public:
	void SetBurnParticleComp(class UParticleSystemComponent* PTComp) { BurnParticleComp = PTComp; }

	float GetSecondDamage() const { return OneSecondDamage; }
	void SetSecondDamage(float fValue) { OneSecondDamage = fValue; }

	/* Member */
private:
	/* 초당 데미지, */
	float OneSecondDamage = 0.0f;
};
