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
	@�����̻�, �ൿ�Ұ� �� ���� ConditionData TArray 
	* UHitUpsetConditionData(�����̻�) , UHitNonActionConditionData(�ൿ�Ұ�)
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

	/* OutDataArray->Empty(����) �� ConditionDatas ��� �����͸� OutDataArray->'Push' */
	//@param - OutDataArray (OUT)
	virtual void GetConditionDatasAfterEmpty(TArray<UConditionData*>* OutDataArray) override;

	virtual void GetConditionDatasWithIndex(TArray<UConditionData*>* OutDataArray, int Index) override;


	/* Function */
public:
	void AddConditionData(UConditionData* ConditionData);

private:
	/* Update - �����̻� */
	//@return - (ApplyTime < 0.0f)
	bool UpdateUpsetCondition(UHitUpsetConditionData * ConditionData);

	/* Update - �ൿ�Ұ� */
	//@return - (ApplyTime < 0.0f)
	bool UpdateNonActionCondition(UHitNonActionConditionData * ConditionData);

	/* UConditionData.ColorAndOpacity Lerp */
	void UpdateColorAndOpacity(UConditionData* ConditionData);

	#pragma	region Member
private:

	#pragma endregion
};
