#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseHitComp.h"
#include "CHM_ShamanHitComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHM_ShamanHitComp 
	: public UC_BaseHitComp
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "CharactorMesh")
		class USkeletalMesh* OriginCharactorMesh;

	UPROPERTY(EditAnywhere, Category = "CharactorMesh")
		class USkeletalMesh* PoisionCharactorMesh;

	UPROPERTY(EditAnywhere, Category = "CharactorMesh")
		class USkeletalMesh* ForDeathCharactorMesh;

	UPROPERTY(EditAnywhere, Category = "Montages")
		class UAnimMontage* NormalHitMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
		class UAnimMontage* AirHitMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
		class UAnimMontage* AirAttackHitMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
		class UAnimMontage* StrongAttackHitMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
		class UAnimMontage* StunHitMontage;

	#pragma endregion

public:
	UCHM_ShamanHitComp();

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
	class ACHM_Shaman* HM_Shaman;

	#pragma endregion
};
