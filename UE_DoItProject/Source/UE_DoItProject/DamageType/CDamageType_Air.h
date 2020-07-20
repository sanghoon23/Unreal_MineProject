#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"
#include "CDamageType_Air.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Air 
	: public UCDamageType_Base
{
	GENERATED_BODY()
	
	#pragma region Reflection
private:

	#pragma endregion

public:
	UCDamageType_Air();

	/* Virtual */
public:
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	/* Function */
public:
	void SetAirAttackHeight(float fValue) { HeightAfterAirAttack = fValue; }

private:
	float HeightAfterAirAttack = 300.0f; //@Default

};
