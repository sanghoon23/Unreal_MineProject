#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_Charactor.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "IC_Player.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset")
		TArray<UCBaseConditionType*> InfoConditionDataArray;

public:
	float MaxHP = 0.0f;
	float CurrentHP = 0.0f;
	float Speed = 0.0f;
	FName Name = "";

	//# 상태 추가..
	//class 
};

UINTERFACE(MinimalAPI)
class UIC_Player : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_Player
{
	GENERATED_BODY()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function */
public:
	virtual const FPlayerInfo GetPlayerInfo() const = 0;

	/* HP 를 올려주는 함수 */
	virtual void HealthUp(float fValue) = 0;

	/* Max HP 를 올려주는 함수 */
	virtual void MaxHealthUp(float fValue) = 0;

	/* Speed 를 올려주는 함수 */
	virtual void SpeedUp(float fValue) = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Member */
public:
	/* 현재 상호작용 하고 있는 Actor 'Get' */
	IIC_InteractActor* GetCurrentInteractActor() { return CurrentInteractActor; }
	/* 현재 상호작용 하고 있는 Actor 'Set' */
	void SetCurrentInteractActor(IIC_InteractActor* IC_Interact) { CurrentInteractActor = IC_Interact; }

protected:
	IIC_InteractActor* CurrentInteractActor;
};
