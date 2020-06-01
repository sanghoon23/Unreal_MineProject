#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Color.h"

#include "DamageType/Base/CDamageType_Base.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "IC_HitComp.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// @HitComponent Interface

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
	/* Pure Virtual Function */
public:
	// @AttackingActor - ������ ��ü Actor
	// @DamageType - ������ ���� (Ex. Normal, Air...) ��
	// @DamageAmount - ��������
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base* const DamageType, float DamageAmount) = 0;

	/* ����� �°� �ִ��� */
	virtual const bool IsBeated() const = 0;

	/* ����� �°� �ִ��� ���� - CN_BeginBeat */
	virtual void SetBeated(bool bValue) = 0;

	/* HitComponent ���� �����̻�, �ൿ�Ұ� ������ */
	// @ArrayNumber - �迭 �����̳��� Index
	virtual UCBaseConditionType* GetConditionData(int Index) = 0;
	
	/* OutDataArray->Empty(����) �� ConditionDatas ��� �����͸� OutDataArray->'Push' */
	//@param - OutDataArray (OUT)
	//@parma Count - ������ ���� (Default -1, ��� ������)
	virtual void GetConditionDatasAfterEmpty(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) = 0;

	/* ConditionDatas �� [Index] ���� ������  */
	virtual void GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index) = 0;

	/* ConditionDatas �� Count(����) ���� ������ */
	//@param OutDataArray - OUT
	//@param Count - ������ ����
	virtual void GetConditionDatasByCount(TArray<UCBaseConditionType*>* OutDataArray, int IndexCount) = 0;

	#pragma region Member
public:
	virtual void SetBlockDamagedMontage(bool bValue) = 0;

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
