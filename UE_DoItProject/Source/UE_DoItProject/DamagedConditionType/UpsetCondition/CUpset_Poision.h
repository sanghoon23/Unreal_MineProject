#pragma once

#include "CoreMinimal.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"
#include "CUpset_Poision.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCUpset_Poision 
	: public UCBaseConditionType
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* OriginMaterial = nullptr;

	#pragma endregion

public:
	UCUpset_Poision();
	
	/* Virtual Function */
public:
	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateConditionOnActor(AActor* Owner, float DeltaTime) override;

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행될 함수 */
	//@BaseConditionType.h
	virtual void EndConditionOnActor(AActor * Owner) override;

	/* Function */
public:
	float GetSecondDamage() const { return OneSecondDamage; }
	void SetSecondDamage(float fValue) { OneSecondDamage = fValue; }

	class UMaterialInterface* GetOriginMaterial() { return OriginMaterial; }
	void SetOriginMaterial(class UMaterialInterface* Mat) { OriginMaterial = Mat; }

	/* Member */
private:
	/* 초당 데미지, */
	float OneSecondDamage = 0.0f;

};
