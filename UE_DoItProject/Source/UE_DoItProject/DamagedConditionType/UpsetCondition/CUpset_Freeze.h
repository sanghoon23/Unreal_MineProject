#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Freeze.generated.h"

/*
@Warning -
이 상태는 무조건 FreezeParticleComp 가 존재한다는 가정에서 실행
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
	/* 해당 Owner 의 HitComp AddCondition 될 때 실행 함수*/
	//@BaseConditionType.h
	virtual void StartCondition(APawn* Owner) override;

	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행될 함수 */
	//@BaseConditionType.h
	virtual void EndCondition(APawn * Owner) override;

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;
	
	/* Function */
public:
	void SetFreezeParticle(class UParticleSystem* PT);
	void SetFreezeUnderParticle(class UParticleSystem* PT);

};
