#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseState.h"
#include "Interface/IC_BaseAttack.h"
#include "State/Player/Base/CPL_BaseState.h"

#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"

#include "CPlayerAttackComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCPlayerAttackComp 
	: public UActorComponent, public IIC_AttackComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		UCPL_BaseState* CurrentAttackState;

	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<UCPL_BaseState*> MagicStateArray;

	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<UCPL_BaseState*> SwordStateArray;

	#pragma endregion

public:	
	UCPlayerAttackComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:

	#pragma	region Member
public:
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() override;
	// virtual IIC_BaseAttack* GetAttackStateMember(uint8 Num) override;

private:

	#pragma endregion
};
