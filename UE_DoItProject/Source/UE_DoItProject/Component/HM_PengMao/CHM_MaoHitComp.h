#pragma once

#include "CoreMinimal.h"
#include "Component/Base/C_BaseHitComp.h"
#include "CHM_MaoHitComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCHM_MaoHitComp 
	: public UC_BaseHitComp
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "CharactorMesh")
		class USkeletalMesh* OriginCharactorMesh;

	UPROPERTY(EditAnywhere, Category = "CharactorMesh")
		class USkeletalMesh* ForDeathCharactorMesh;

	UPROPERTY(EditAnywhere, Category = "Montages")
		class UAnimMontage* NormalHitMontage;

	#pragma endregion
	
public:
	UCHM_MaoHitComp();

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
	class ACHM_PengMao* HM_PengMao;

#pragma endregion

};
