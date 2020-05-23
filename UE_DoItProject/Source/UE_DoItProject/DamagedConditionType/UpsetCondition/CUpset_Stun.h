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

	#pragma endregion

public:
	UCUpset_Stun();

	/* Virtual Function */
public:
	virtual void UpdateCondition(APawn* Owner, float DeltaTime) override;

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) override;

	/* Member */
public:
	void SetMontage(class UAnimMontage* Montage);

};
