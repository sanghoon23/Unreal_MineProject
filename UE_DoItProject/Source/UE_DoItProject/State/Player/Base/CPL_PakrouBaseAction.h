#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseActionState.h"
#include "Component/Player/CPL_PakrouActionComp.h"

#include "CPL_PakrouBaseAction.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_PakrouBaseAction 
	: public UC_BaseActionState
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:

	#pragma endregion

public:
	UCPL_PakrouBaseAction();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Interface Pure Function */
public:
	virtual void OnAction() override {}

public:
	virtual void BeginActionState() override {}
	virtual void TickActionState() override {}
	virtual void EndActionState() override {}

	/* Function */
public:
	// @대상 설정
	void SetPakrouObject(AActor* Actor) { PakrouObject = Actor; }

	// @Component 설정
	void SetPakrouComponent(class UCPL_PakrouActionComp* PakrouComponent) { PakrouActionComp = PakrouComponent; }

protected:
	void SetTargetDistance();
	bool CheckClimbingPossible(FVector CheckLocation);

	#pragma	region Member
protected:
	class ACPlayer* Player;
	AActor* PakrouObject;

	// @PakrouComponent Pointer
	class UCPL_PakrouActionComp* PakrouActionComp;

	#pragma endregion
};
