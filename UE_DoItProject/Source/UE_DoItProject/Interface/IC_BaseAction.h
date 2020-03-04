#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_BaseAction.generated.h"

UINTERFACE(MinimalAPI)
class UIC_BaseAction : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_BaseAction
{
	GENERATED_BODY()

	/* Pure Virtual Function */
public:
	virtual void OnAction() = 0;

	/* Virtual Function */
public:
	virtual void BeginActionState() {}
	virtual void TickActionState() {}
	virtual void EndActionState() {}

public:
	// @Onwer - Action 하는 Pawn (주체)
	// Protected - OwnerPawn 에 넣어짐.
	// 좀 더 편하게 Owner 로 접근하기 위해 만듬.
	void SetOwnerPawn(APawn* Owner) { OwnerPawn = Owner; }

protected:
	APawn* OwnerPawn;
};
