#pragma once

#include "CoreMinimal.h"
#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "DamageType/CDamageType_Normal.h"

#include "CHM_BasicFirstCombo.generated.h"

UENUM()
enum class UHM_BasicFirstComboType : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_BasicFirstCombo 
	: public UCHM_BasicBaseAttack
{
	GENERATED_BODY()
	#pragma region Reflection
private:
	// @DamageType
	UPROPERTY(VisibleAnywhere, Category = "DamageType")
		UCDamageType_Normal*	NormalDamageType;

	#pragma endregion

public:
	UCHM_BasicFirstCombo();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	//
public:
	void BeginAttack(AActor * DoingActor) override;
	void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;

	void AttackOtherPawn() override;

	#pragma region Member
private:
	class ACHM_Basic*	HM_Basic;

	float AttackRadius = 100.0f;

	#pragma endregion
};
