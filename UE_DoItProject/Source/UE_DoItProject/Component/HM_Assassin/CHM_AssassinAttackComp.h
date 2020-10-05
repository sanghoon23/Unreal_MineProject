#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "CHM_AssassinAttackComp.generated.h"

UENUM()
enum class EHM_AssassinAttackType : uint8
{
	FIRSTATTACK = 0, //@Test
	SECONDATTACK = 1,
	THIRDATTACK = 2,
	FOURATTACK = 3,
	FIVEATTACK = 4,
	SIXATTACK = 5,
	END = 6,
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

	virtual void GetBaseAttackList(TArray<class UC_BaseAttackState*>& Out) override;
	virtual UC_BaseAttackState* GetBaseAttack(uint8 Type) override;

#pragma	region Member
private:
	EHM_AssassinAttackType AttackType = EHM_AssassinAttackType::FIRSTATTACK;

#pragma endregion

};
