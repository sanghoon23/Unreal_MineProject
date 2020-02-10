#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_SDBasicAttack.generated.h"

UENUM()
enum class USD_BasicAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SDBasicAttack 
	: public UCPL_SwordBaseAttack
{
	GENERATED_BODY()
	
public:
	UCPL_SDBasicAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	//
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void OnComboSet(AActor * DoingActor) override;
};
