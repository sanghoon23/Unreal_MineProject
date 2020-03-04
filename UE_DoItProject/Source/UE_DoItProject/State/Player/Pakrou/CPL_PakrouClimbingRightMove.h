#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_PakrouBaseAction.h"
#include "CPL_PakrouClimbingRightMove.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_PakrouClimbingRightMove : public UCPL_PakrouBaseAction
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* ClimbingRightMoveMontage;

	#pragma endregion

public:
	UCPL_PakrouClimbingRightMove();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - Interface */
public:
	virtual void OnAction() override;

	/* Virtual Function */
public:
	virtual void BeginActionState() override;
	virtual void TickActionState() override;
	virtual void EndActionState() override;
	
private:
	float ClimbingRightMoveSpeed = 1.0f;
};
