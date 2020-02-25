#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "CPL_ActionJumpOver.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_ActionJumpOver 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* JumpOverMontage;

	#pragma endregion

public:
	UCPL_ActionJumpOver();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - Interface */
public:
	virtual void OnAction() override;

	/* Member */
private:
	class ACPlayer* Player;

};
