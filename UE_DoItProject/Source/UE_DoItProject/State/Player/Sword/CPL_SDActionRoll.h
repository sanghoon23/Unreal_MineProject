#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "CPL_SDActionRoll.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SDActionRoll
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* RollMontage;

	#pragma endregion

public:
	UCPL_SDActionRoll();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function */
public:
	void OnAction() override;

	/* Member */
private:
	class ACPlayer* Player;

	float UsingActionRange = 200.0f;
};
