#pragma once

#include "CoreMinimal.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "CDamageType_Normal.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Normal 
	: public UCDamageType_Base
{
	GENERATED_BODY()

	#pragma region Reflection
private:

	#pragma endregion

public:
	UCDamageType_Normal();

	/* Virtual */
public:
	//@param Subject - ��ü��
	//@param DamagedActor - �´� ����
	//@param InitialDamageAmount - �ʱ⿡ �� ������ ��
	//@param Montage - �´� ���Ͱ� ������ Montage (default = nullptr)
	virtual void OnHittingProcess(AActor* Subject, AActor* DamagedActor, class UC_BaseHitComp* DamagedActorHitComp, float InitialDamageAmount);

	virtual void SettingData(const FDamageData& Data) override;
};
