#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_StateManager.generated.h"

UINTERFACE(MinimalAPI)
class UIC_StateManager : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE(FOnMageState)
DECLARE_MULTICAST_DELEGATE(FUnMageState)

DECLARE_MULTICAST_DELEGATE(FOnSwordState)
DECLARE_MULTICAST_DELEGATE(FUnSwordState)

class UE_DOITPROJECT_API IIC_StateManager
{
	GENERATED_BODY()

public:
	// @Mage ���·� ' ���� ' �� ��
	FOnMageState	OnMageState;

	// @Mage ���°� ' ���� ' �� ��
	FUnMageState	UnMageState;

	// @Sword ���·� ' ���� ' �� ��
	FOnSwordState	OnSwordState;

	// @Sword ���°� ' ���� ' �� ��
	FUnSwordState	UnSwordState;
};
