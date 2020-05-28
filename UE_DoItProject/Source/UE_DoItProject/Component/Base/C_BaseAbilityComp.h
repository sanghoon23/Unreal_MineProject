#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/IC_AbilityComp.h"

#include "C_BaseAbilityComp.generated.h"


UCLASS()
class UE_DOITPROJECT_API UC_BaseAbilityComp 
	: public UActorComponent, public IIC_AbilityComp
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	TMap<EAbilityType, UCBaseAbility*> AddAbilityMap;

	#pragma endregion



public:	
	UC_BaseAbilityComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Pure Virtual Function - IIC_AbilityComp */
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
