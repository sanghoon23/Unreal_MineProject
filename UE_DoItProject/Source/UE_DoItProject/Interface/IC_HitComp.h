#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageType/Base/CDamageType_Base.h"

#include "IC_HitComp.generated.h"

UINTERFACE(MinimalAPI)
class UIC_HitComp : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitFunc, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitResetState, AActor*)

class UE_DOITPROJECT_API IIC_HitComp
{
	GENERATED_BODY()

public:
	// - Hit ���� ��, ������ �Լ� Delegate
	// @param AActor* - �Լ��� ������ Hitted(Hit ���ϴ� ) ��ü
	// FOnHitFunc				OnHitFunc;

	// - Hit ���� ��, ���� ���°����� ������ �ǵ����� Delegate
	// @param AActor* - Hitted( Hit ���ϴ� ) ��ü
	// FOnHitResetState		OnHitResetState;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	// @AttackingActor - ������ ��ü Actor
	// @DamageType - ������ ���� (Ex. Normal, Air...) ��
	// @DamageAmount - ��������
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base* const DamageType, float DamageAmount) = 0;


	#pragma region Member
public:
	/* Hit ������ ��, ������ '����' */
	FVector GetHitDirection() { return HitDirection; }

	/* Hit ������ ��, ������ '����' */
	void SetHitDirection(FVector Direction) { HitDirection = Direction; }

	/* Hit ������ ��, ������ '�ӵ�' */
	float GetHitMoveSpeed() const { return HitMoveSpeed; }

	/* Hit ������ ��, ������ '�ӵ�' */
	void SetHitMoveSpeed(float fValue) { HitMoveSpeed = fValue; }

protected:
	FVector HitDirection = FVector(0.0f);
	float HitMoveSpeed = 1.0f; //@Charactor Movement �� ���. (Default=1.0f)

	#pragma endregion
};
