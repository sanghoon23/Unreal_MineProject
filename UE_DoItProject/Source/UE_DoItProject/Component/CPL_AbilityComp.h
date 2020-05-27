#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_AbilityComp.h"

#include "CPL_AbilityComp.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCPL_AbilityComp
	: public UActorComponent, public IIC_AbilityComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	TMap<EAbilityType, UCBaseAbility*> AddAbilityMap;

	// Test Code
	//TMultiMap<EAddAttribute, FAddAbilityValue> AddAbilityMultiMap;
	//TArray<UCBaseAbility*> AddedAbilityArray;

	#pragma endregion

public:	
	UCPL_AbilityComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function */
public:
	/* 능력치 적용 */
	//@두가지 상황을 적용
	//1. Timer 가 쓰이는 능력치 - Ex) 잠시 이동속도 증가, 공격 증가 등등..
	//만약 Timer 가 쓰인다면 AddAbilityMap Emplace 
	//
	//2. Timer 가 쓰이지 않고 영구적으로 증가
	virtual void AddAbility(UCBaseAbility* Ability) override;

	/* Memeber */
private:

};
