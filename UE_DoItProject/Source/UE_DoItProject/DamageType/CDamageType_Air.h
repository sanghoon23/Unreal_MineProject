#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Air.generated.h"

UCLASS(BlueprintType)
class UE_DOITPROJECT_API UCDamageType_Air 
	: public UCDamageType_Base
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Data")
		float HeightAfterAirAttack = 300.0f; //@Default

	#pragma endregion

public:
	UCDamageType_Air();

	/* Virtual */
public:
	//@param Subject - 주체자
	//@param DamagedActor - 맞는 액터
	//@param InitialDamageAmount - 초기에 들어갈 데미지 값
	//@param Montage - 맞는 액터가 시행할 Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	virtual void SettingData(const FDamageData& Data) override;

	/* Function */
public:
	void SetAirAttackHeight(float fValue) { HeightAfterAirAttack = fValue; }

private:

};
