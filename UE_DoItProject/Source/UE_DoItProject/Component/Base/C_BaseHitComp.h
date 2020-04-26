#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_HitComp.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "C_BaseHitComp.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @HitComponent

UCLASS( ClassGroup=(Custom) )
class UE_DOITPROJECT_API UC_BaseHitComp 
	: public UActorComponent, public IIC_HitComp
{
	GENERATED_BODY()

private:
	const int DataNum = 5;
	const float OpacityLinearTimer = 3.0f;
	const float OpacityLinearSpeed = 0.01f;

	#pragma	region Reflection
private:
	/*
	@상태이상, 행동불가 를 다음 ConditionData TArray 
	* UHitUpsetConditionData(상태이상) , UHitNonActionConditionData(행동불가)
	*/
	UPROPERTY(VisibleAnywhere, Category = "ConditionData")
	TArray<UConditionData*> ConditionDatas;

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

	virtual UConditionData* GetConditionData(int Index) override;

	/* OutDataArray->Empty(갱신) 후 ConditionDatas 모든 데이터를 OutDataArray->'Push' */
	//@param - OutDataArray (OUT)
	virtual void GetConditionDatasAfterEmpty(TArray<UConditionData*>* OutDataArray) override;

	virtual void GetConditionDatasWithIndex(TArray<UConditionData*>* OutDataArray, int Index) override;


	/* Function */
public:
	void AddConditionData(UConditionData* ConditionData);

private:
	/* Update - 상태이상 */
	//@return - (ApplyTime < 0.0f)
	bool UpdateUpsetCondition(UHitUpsetConditionData * ConditionData);

	/* Update - 행동불가 */
	//@return - (ApplyTime < 0.0f)
	bool UpdateNonActionCondition(UHitNonActionConditionData * ConditionData);

	/* UConditionData.ColorAndOpacity Lerp */
	void UpdateColorAndOpacity(UConditionData* ConditionData);

	#pragma	region Member
private:

	#pragma endregion
};
