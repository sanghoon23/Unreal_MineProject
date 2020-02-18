#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "CPL_MGActionJump.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_MGActionJump 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
		class UAnimMontage* JumpMontage;

	#pragma endregion

public:
	UCPL_MGActionJump();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	void OnAction(AActor* DoingActor) override;
};
