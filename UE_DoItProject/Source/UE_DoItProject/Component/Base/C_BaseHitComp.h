#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_HitComp.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "C_BaseHitComp.generated.h"

USTRUCT()
struct FHitUpsetConditionData
{
	GENERATED_USTRUCT_BODY()

	float ApplyTime = 0.0f;
	class UAnimMontage* UpsetConditionMon;
};

USTRUCT()
struct FHitNonActionConditionData
{
	GENERATED_USTRUCT_BODY()

public:
	FHitNonActionConditionData() {}

public:
	float ApplyTime = 0.0f;
	class UAnimMontage* NonActionMon = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UC_BaseHitComp 
	: public UActorComponent, public IIC_HitComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "Montages")
		TArray<FHitUpsetConditionData> UpsetConditionDatas;

	UPROPERTY(VisibleAnywhere, Category = "Montages")
		FHitNonActionConditionData NonActionConditionData;

	#pragma endregion

public:	
	UC_BaseHitComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function */
public:
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base * const DamageType, float DamageAmount) override {}

	/* Function */
public:
	void AddUpsetCondition(const FHitUpsetConditionData& ConditionData);
	void AddNonActionCondition(const FHitNonActionConditionData& ConditionData);

	#pragma	region Member
private:
	bool bNonAction = false;

	#pragma endregion
};
