#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_BaseAction.h"

#include "C_BaseActionState.generated.h"


UCLASS()
class UE_DOITPROJECT_API UC_BaseActionState 
	: public UActorComponent, public IIC_BaseAction
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UC_BaseActionState();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Interface Pure Function */
public:
	virtual void OnAction() override {}

public:
	virtual void BeginActionState() override {}
	virtual void TickActionState() override {}
	virtual void EndActionState() override {}

	#pragma	region Member
private:

	#pragma endregion
};
