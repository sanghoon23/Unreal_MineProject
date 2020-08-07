#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "CHM_AssassinAttackComp.generated.h"

UENUM()
enum class HM_AssassinAttackType
{
	FIRSTATTACK = 0, //@Test
	SECONDATTACK = 1,
	THIRDATTACK = 2,
	FOURATTACK = 3,
	END = 4,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_AssassinAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<class UC_BaseAttackState*> BasicAttackStateArray;

#pragma endregion

public:
	UCHM_AssassinAttackComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	IIC_BaseAttack* GetCurrentIBaseAttack() override;

#pragma	region Member
private:
	HM_AssassinAttackType AttackType = HM_AssassinAttackType::FIRSTATTACK;

#pragma endregion

};
