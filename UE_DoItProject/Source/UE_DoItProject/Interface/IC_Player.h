#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_Charactor.h"

#include "IC_Player.generated.h"

UINTERFACE(MinimalAPI)
class UIC_Player : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_Player
{
	GENERATED_BODY()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Function */
public:

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Member */
		/* ���� ��ȣ�ۿ� �ϰ� �ִ� Actor 'Get' */
	IIC_InteractActor* GetCurrentInteractActor() { return CurrentInteractActor; }
	/* ���� ��ȣ�ۿ� �ϰ� �ִ� Actor 'Set' */
	void SetCurrentInteractActor(IIC_InteractActor* IC_Interact) { CurrentInteractActor = IC_Interact; }

protected:
	IIC_InteractActor* CurrentInteractActor;
};
