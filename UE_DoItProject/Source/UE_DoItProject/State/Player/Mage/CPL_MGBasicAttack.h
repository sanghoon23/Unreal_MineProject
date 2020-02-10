#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_MGBasicAttack.generated.h"

UENUM()
enum class UMG_BasicAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_MGBasicAttack 
	: public UCPL_MageBaseAttack
{
	GENERATED_BODY()
	
public:
	UCPL_MGBasicAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	//
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void OnComboSet(AActor * DoingActor) override;
};
