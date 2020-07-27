#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseAttackComp.h"
#include "Interface/IC_BaseAttack.h"

#include "CHM_MaoAttackComp.generated.h"

UENUM()
enum class HM_PengMaoAttackType
{
	FIRSTATTACK = 0, //@연속 공격
	SECONDATTACK = 1, //@내려찍고 빙결
	THIRDATTACK = 2, //@WindMil
	FOURATTACK = 3, //@타원형 범위 스킬 공격
	END = 4,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoAttackComp 
	: public UC_BaseAttackComp
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	//UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
	//	class USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, Category = "BaseAttack")
		TArray<class UC_BaseAttackState*> BasicAttackStateArray;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class UCHM_MaoFirstAttack* HMMao_FirstAttack;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class UCHM_MaoSecondAttack* HMMao_SecondAttack;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class UCHM_MaoThirdAttack* HMMao_ThirdAttack;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//	class UCHM_MaoFourAttack* HMMao_Four;

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
