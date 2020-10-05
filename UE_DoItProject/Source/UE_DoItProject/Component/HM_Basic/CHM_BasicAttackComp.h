#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_BasicAttackComp.generated.h"

UENUM()
enum class EHM_BasicAttackType : uint8
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
		TArray<class UC_BaseAttackState*> BasicAttackStateArray;

	#pragma endregion

public:
	UCHM_BasicAttackComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	IIC_BaseAttack* GetCurrentIBaseAttack() override;

	virtual void GetBaseAttackList(TArray<class UC_BaseAttackState*>& Out) override {};

#pragma	region Member
private:
	EHM_BasicAttackType AttackType = EHM_BasicAttackType::BASIC;

#pragma endregion
};
