#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHM_AbilityComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCHM_AbilityComp 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion



public:	
	UCHM_AbilityComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
