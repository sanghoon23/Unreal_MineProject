#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "C_BaseAttackComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UC_BaseAttackComp
	: public UActorComponent, public IIC_AttackComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UC_BaseAttackComp();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override { return nullptr; }
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() override { return nullptr; }


	#pragma	region Member
protected:

	#pragma endregion
};
