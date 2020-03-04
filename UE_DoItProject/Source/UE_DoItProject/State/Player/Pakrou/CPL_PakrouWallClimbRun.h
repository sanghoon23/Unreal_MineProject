#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_PakrouBaseAction.h"

#include "CPL_PakrouWallClimbRun.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_PakrouWallClimbRun 
	: public UCPL_PakrouBaseAction
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* WallClimbRunMontage;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* NextClimbingIdleMontage;

	#pragma endregion

public:
	UCPL_PakrouWallClimbRun();
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
	float WallClimbingSpeed = 1.0f;
};
