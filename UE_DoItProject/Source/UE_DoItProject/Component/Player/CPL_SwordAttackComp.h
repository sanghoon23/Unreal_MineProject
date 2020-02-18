#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component/Base/C_BaseAttackComp.h"

#include "CPL_SwordAttackComp.generated.h"

UENUM()
enum class SwordAttackType // @SwordType - Sword 상태일 때의 공격 Type
{
	BASIC = 0,
	UPPER = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SwordAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<class UCPL_SwordBaseAttack*> SwordAttackStateArray;

	#pragma endregion

public:	
	UCPL_SwordAttackComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	IIC_BaseAttack* GetCurrentIBaseAttack() override;

	#pragma	region Member
private:
	SwordAttackType AttackType = SwordAttackType::END;

	#pragma endregion
};
