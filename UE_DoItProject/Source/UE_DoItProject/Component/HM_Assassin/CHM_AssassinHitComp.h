#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseHitComp.h"
#include "CHM_AssassinHitComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHM_AssassinHitComp 
	: public UC_BaseHitComp
{
	GENERATED_BODY()
	
#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Material")
		class UMaterialInterface* Mat_Poision_0;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		class UAnimMontage* NormalHitMontage;

#pragma endregion

public:
	UCHM_AssassinHitComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function */
public:
	void OnHit(AActor* AttackingActor, UCDamageType_Base * DamageType, float DamageAmount) override;

	/* Function */
private:

#pragma region Member
private:
	class ACHM_Assassin* HM_Assassin;

#pragma endregion


};
