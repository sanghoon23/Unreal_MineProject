#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IC_HitComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_HitComp : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitResetState, AActor*)

class UE_DOITPROJECT_API IIC_HitComp
{
	GENERATED_BODY()

public:
	// @Hit ���� ��, ���� ���°����� ������ �ǵ����� Delegate
	FOnHitResetState		OnHitResetState;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	virtual void OnHit(AActor* AttackActor, UINT HitAnimNum, float AnimSpeed) {}

};
