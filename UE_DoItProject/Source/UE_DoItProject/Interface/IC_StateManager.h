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
	// @Mage 상태로 ' 변경 ' 될 때
	FOnMageState	OnMageState;

	// @Mage 상태가 ' 해제 ' 될 때
	FUnMageState	UnMageState;

	// @Sword 상태로 ' 변경 ' 될 때
	FOnSwordState	OnSwordState;

	// @Sword 상태가 ' 해제 ' 될 때
	FUnSwordState	UnSwordState;
};
