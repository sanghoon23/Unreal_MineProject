#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "CPL_SDRollAction.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SDRollAction 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		class UAnimMontage* RollMontage;

	#pragma endregion

public:
	UCPL_SDRollAction();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnAction(AActor* DoingActor) override;
};
