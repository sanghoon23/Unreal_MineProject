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
	/* �ɷ�ġ ���� */
	//@�ΰ��� ��Ȳ�� ����
	//1. Timer �� ���̴� �ɷ�ġ - Ex) ��� �̵��ӵ� ����, ���� ���� ���..
	//���� Timer �� ���δٸ� AddAbilityMap Emplace 
	//
	//2. Timer �� ������ �ʰ� ���������� ����
	virtual void AddAbility(UCBaseAbility* Ability) override;

	/* Memeber */
private:

};
