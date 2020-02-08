#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_AttackComp.h"

#include "C_BaseAttackComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UC_BaseAttackComp
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UC_BaseAttackComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() { return nullptr; }

	#pragma	region Member
protected:

	#pragma endregion
};
