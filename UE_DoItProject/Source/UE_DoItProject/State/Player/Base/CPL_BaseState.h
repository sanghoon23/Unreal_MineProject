#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseState.h"

#include "CPL_BaseState.generated.h"

UENUM()
enum class StateType : uint8
{
	MAGE = 0,
	SWORD = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_BaseState
	: public UActorComponent, public IIC_BaseState
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCPL_BaseState();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual IIC_BaseAttack* GetIBaseAttack() override;

	#pragma	region Member
protected:
	StateType StateType = StateType::END;

private:

	#pragma endregion
};
