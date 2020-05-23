#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Poision.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCUpset_Poision 
	: public UCBaseConditionType
{
	GENERATED_BODY()

private:
	const float DamagedTime = 1.0f;

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* OriginMaterial = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		/* 시간 마다 들어갈 데미지, */
		float SecondDamage = 0.0f;

	#pragma endregion

public:
	UCUpset_Poision();
	
	/* Virtual Function */
public:
	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행될 함수 */
	//@BaseConditionType.h
	virtual void EndCondition(APawn * Owner) override;

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* Function */
public:
	float GetSecondDamage() const { return SecondDamage; }
	void SetSecondDamage(float fValue) { SecondDamage = fValue; }

	void SetOriginMaterial(class UMaterialInterface* Mat);

	/* Member */
private:
	/* Timer */
	float SecondTimer = 0.0f;

};
