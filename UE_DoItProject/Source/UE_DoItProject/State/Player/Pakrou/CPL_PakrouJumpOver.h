#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_PakrouBaseAction.h"

#include "CPL_PakrouJumpOver.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_PakrouJumpOver 
	: public UCPL_PakrouBaseAction
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* JumpOverMontage;

	#pragma endregion

public:
	UCPL_PakrouJumpOver();
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

	/* Member */
private:
	float JumpOverSpeed = 0.1f;
};
