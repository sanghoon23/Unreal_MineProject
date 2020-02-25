#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "CPL_SDActionJump.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_SDActionJump 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* JumpMontage;

	#pragma endregion

public:
	UCPL_SDActionJump();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void OnAction() override;

	/* Member */
private:
	class ACPlayer* Player;
};
