#pragma once

#include "CoreMinimal.h"
#include "State/Base/C_BaseAttackState.h"
#include "Interface/IC_Component.h"

#include "CHM_AssaThirdAttack.generated.h"

UENUM()
enum class EHM_AssaThirdComboType : uint8
{
	NONE = 0,
	COMBO_ONE = 1,
	COMBO_TWO = 2,
	END = 3,
};

UCLASS()
class UE_DOITPROJECT_API UCHM_AssaThirdAttack
	: public UC_BaseAttackState, public IIC_Component
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<class UAnimMontage*> AttackMontages;

#pragma endregion

public:
	UCHM_AssaThirdAttack();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	/* Pure Virtual Function - IC_Component */
	virtual void IsRunTick(bool bRunning) override;

	/* Virtual - IC_BaseAttack */
public:
	virtual void BeginAttack(AActor * DoingActor) override;
	virtual void OnComboSet(AActor * DoingActor) override;

	//virtual bool IsLastCombo() const override;
	//virtual void AttackOtherPawn() override;
	
#pragma region Member
private:
	class ACHM_Assassin* HM_Assassin;
	class IIC_Charactor* I_Charactor;

	TArray<class UCN_SpawnProjectile*> Notifies_SpawnProjectile;

#pragma endregion

};
