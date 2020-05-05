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
	/* �ش� Owner �� HitComp Tick ���� Update */
	virtual void UpdateConditionOnActor(AActor* Owner, float DeltaTime) override;

	/* �ش� ���°� ��ø�Ǿ��� ��, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* �ش� Onwer �� HitComp ���� ApplyTime �� ���� ��, ����� �Լ� */
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
	/* �ʴ� ������, */
	float OneSecondDamage = 0.0f;

};
