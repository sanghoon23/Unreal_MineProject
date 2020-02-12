#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "State/HM_Basic/Base/CHM_BasicBaseAttack.h"
#include "Interface/IC_AttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_BasicAttackComp.generated.h"

UENUM()
enum class HM_BasicAttackType // @MageType - Mage 상태일 때의 공격 Type
{
	BASIC = 0,
	END = 1,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_BasicAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<UCHM_BasicBaseAttack*> AttackStateArray;

	#pragma endregion

public:
	UCHM_BasicAttackComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	virtual IIC_BaseAttack* GetCurrentIBaseAttack() override;

#pragma	region Member
private:
	HM_BasicAttackType AttackType = HM_BasicAttackType::END;

#pragma endregion
};
