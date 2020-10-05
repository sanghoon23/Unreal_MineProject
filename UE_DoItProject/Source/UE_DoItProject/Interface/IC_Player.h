#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "interface/IC_WidgetInfo.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_AbilityComp.h"

#include "IC_Player.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset")
		TArray<FViewConditionState> InfoConditionDataArray;

public:
	float MaxHP				= 0.0f;
	float CurrentHP			= 0.0f;

	float MaxMP				= 0.0f;
	float CurrentMP			= 0.0f;

	float BarrierAmount		= 0.0f;

	float AddSpeed			= 0.0f;
	float ATK_Coefficient	= 1.0f; //@공격 지수
	float DEF_Coefficient	= 1.0f; //@방어 지수

	FName Name			= "";

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
	virtual void OnBlockKeyInput() = 0;
	virtual void OffBlockKeyInput() = 0;

	virtual void OnBlockAction() = 0;
	virtual void OffBlockAction() = 0;

	virtual void OnParticleInPlayer() = 0;
	virtual void OffParticleInPlayer() = 0;

	virtual const FPlayerInfo& GetPlayerInfo() const = 0;

	virtual APawn* GetFindAttackTarget() = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	virtual IIC_StateManager* GetIStateManager() { return nullptr; }


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
