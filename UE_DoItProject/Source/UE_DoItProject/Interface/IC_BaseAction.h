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
	// @Onwer - Action �ϴ� Pawn (��ü)
	// Protected - OwnerPawn �� �־���.
	// �� �� ���ϰ� Owner �� �����ϱ� ���� ����.
	void SetOwnerPawn(APawn* Owner) { OwnerPawn = Owner; }

protected:
	APawn* OwnerPawn;
};
