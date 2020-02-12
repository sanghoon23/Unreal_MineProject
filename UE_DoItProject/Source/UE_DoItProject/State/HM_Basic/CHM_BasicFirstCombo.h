#pragma once

#include "CoreMinimal.h"
#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

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
	
public:
	UCHM_BasicFirstCombo();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	//
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void OnComboSet(AActor * DoingActor) override;
	virtual bool IsLastCombo() const override;
};
