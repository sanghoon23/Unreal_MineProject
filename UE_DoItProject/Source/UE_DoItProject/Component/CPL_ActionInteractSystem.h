#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "State/Base/C_BaseActionState.h"

#include "CPL_ActionInteractSystem.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCPL_ActionInteractSystem 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Pakrou")
		UC_BaseActionState* JumpOver;

	#pragma endregion

public:	
	UCPL_ActionInteractSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	void InteractInput();

private:
	// void 

	#pragma	region Member
private:
	class ACPlayer* Player;

	float InteractDistance = 100.0f;
	float InteractRadius = 40.0f;
	#pragma endregion
};
