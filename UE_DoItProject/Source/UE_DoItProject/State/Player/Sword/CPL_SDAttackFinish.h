#pragma once

#include "CoreMinimal.h"
#include "State/Player/Base/CPL_SwordBaseAttack.h"
#include "Interface/IC_Component.h"

#include "DamageType/CDamageType_Normal.h"
#include "DamageType/CDamageType_StrongAttack.h"

#include "CPL_SDAttackFinish.generated.h"

UENUM()
enum class ESD_FinalAttack : uint8
{
	COMBO_ONE = 0,
	COMBO_TWO = 1,
	COMBO_THREE = 2,
	COMBO_FOUR = 3,
	COMBO_FIVE = 4,
	END = 5,
};

UCLASS()
class UE_DOITPROJECT_API UCPL_SDAttackFinish 
	: public UCPL_SwordBaseAttack, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:

#pragma endregion

public:
	UCPL_SDAttackFinish();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Pure Virtual Function */
public:
	void BeginAttack(AActor * DoingActor) override;
	void EndAttack() override;

	//void OnComboSet(AActor * DoingActor) override;
	bool IsLastCombo() const override;

	/* Virtual */
public:
	void AttackOtherPawn(UCDamageType_Base* DamageType) override;
	void ImpulseAttack(float intensity) override {}
	void CheckProcedural() override {}
	
	#pragma region Member
private:
	float AttackRadius = 100.0f;

	APawn* Target;

	//@UI
	class UWG_TargetInfo* TargetInfoWidget;

	#pragma endregion
};
