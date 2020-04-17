#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CPL_MageAttackComp.generated.h"

UENUM()
enum class EMageAttackType : uint8// @MageType - Mage 상태일 때의 공격 Type
{
	BASIC				= 0,
	MAGICBALLATTACK		= 1,
	END					= 2,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_MageAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<class UCPL_MageBaseAttack*> MageAttackStateArray;

	#pragma endregion

public:	
	UCPL_MageAttackComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	IIC_BaseAttack* GetCurrentIBaseAttack() override;

	#pragma	region Member
public:

private:
	EMageAttackType AttackType = EMageAttackType::BASIC;

	#pragma endregion
};
