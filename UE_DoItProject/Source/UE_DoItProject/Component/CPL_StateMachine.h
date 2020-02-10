#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_StateManager.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_ActionComp.h"

#include "Component/Base/C_BaseAttackComp.h"
#include "Component/Base/C_BaseActionComp.h"

#include "CPL_StateMachine.generated.h"

UENUM()
enum class PlayerStateType : uint8
{
	MAGE = 0,
	SWORD = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_StateMachine 
	: public UActorComponent, public IIC_StateManager
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
		TArray<class UC_BaseAttackComp*> AttackCompArray;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		TArray<class UC_BaseActionComp*> ActionCompArray;

	UPROPERTY(VisibleInstanceOnly, Category = "Montages")
		class UAnimMontage* MageStateCastMontage;

	#pragma endregion

public:	
	UCPL_StateMachine();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	IIC_AttackComp* GetIAttackComp();
	IIC_ActionComp* GetIActionComp();
		
public:
	void OnSwapState();

	#pragma	region Member
public:
	PlayerStateType GetCurrentStateType() { return CurrentStateType; }

private:
	IIC_Charactor* Charactor;
	PlayerStateType CurrentStateType = PlayerStateType::MAGE;

	#pragma endregion
};
