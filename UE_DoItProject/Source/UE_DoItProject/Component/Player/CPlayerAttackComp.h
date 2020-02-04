#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"
#include "State/Player/Base/CPL_BaseAttack.h"

#include "CPlayerAttackComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UCPlayerAttackComp 
	: public UActorComponent, public IIC_AttackComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		UCPL_BaseAttack* CurrnetAttackState;

	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<UCPL_BaseAttack*> MagicStateArray;

	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<UCPL_BaseAttack*> SwordStateArray;

	#pragma endregion

public:	
	UCPlayerAttackComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:	

		

	#pragma	region Member
public:
	virtual IIC_BaseAttack* GetCurrentAttackState() override { return Cast<IIC_BaseAttack>(CurrnetAttackState); }
	virtual IIC_BaseAttack* GetAttackStateMember(uint8 Num) override;

private:

	#pragma endregion
};
