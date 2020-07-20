#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_MaoAttackComp.generated.h"

UENUM()
enum class HM_PengMaoAttackType
{
	FIRSTATTACK = 0, //@¿¬¼Ó °ø°Ý
	SECONDATTACK = 1, //@³»·ÁÂï°í ºù°á
	THIRDATTACK = 2, //@WindMil
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<class UC_BaseAttackState*> BasicAttackStateArray;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	UC_BaseAttackState* HMMao_FirstAttack;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	UC_BaseAttackState* HMMao_SecondAttack;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	UC_BaseAttackState* HMMao_ThirdAttack;

	#pragma endregion
	
public:
	UCHM_MaoAttackComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
	IIC_BaseAttack* SetAttackTypeRetIBaseAttack(uint8 Type) override;
	IIC_BaseAttack* GetCurrentIBaseAttack() override;

	#pragma	region Member
private:
	HM_PengMaoAttackType AttackType = HM_PengMaoAttackType::FIRSTATTACK;

	#pragma endregion

};
