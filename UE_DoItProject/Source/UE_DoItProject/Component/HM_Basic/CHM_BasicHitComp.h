#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseHitComp.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "CHM_BasicHitComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHM_BasicHitComp 
	: public UC_BaseHitComp
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* NormalHit;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* AirHit;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* AirAttackHit;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* StrongAttackHit;

	#pragma endregion

public:
	UCHM_BasicHitComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function */
public:
	void OnHit(AActor* AttackingActor, UCDamageType_Base * const DamageType, float DamageAmount) override;

	//
	/* Function */
private:
	void LookAtActor(AActor* Target);

	#pragma region Member
private:
	class ACHM_Basic* HM_Basic;

	#pragma endregion
};
