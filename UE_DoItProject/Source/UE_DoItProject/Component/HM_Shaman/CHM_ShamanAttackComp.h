#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_ShamanAttackComp.generated.h"

UENUM()
enum class HM_ShamanAttackType
{
	FIRSTATTACK = 0,
	SECONDATTACK = 1,
	END = 2,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_ShamanAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<class UC_BaseAttackState*> AttackStateArray;

	#pragma endregion

public:
	UCHM_ShamanAttackComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	IIC_BaseAttack* GetCurrentIBaseAttack() override;

	#pragma	region Member
private:
	HM_ShamanAttackType AttackType = HM_ShamanAttackType::FIRSTATTACK;

	#pragma endregion

};
