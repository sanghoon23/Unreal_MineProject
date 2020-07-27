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

DECLARE_MULTICAST_DELEGATE_OneParam(FBeginBeatedFunc, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FEndBeatedFunc, AActor*)

class UE_DOITPROJECT_API IIC_HitComp
{
	GENERATED_BODY()

public:
	// - Hit Montage ���� ��, CN_BeginBeated ���� ���
	FBeginBeatedFunc BeginBeatedFunc;

	// @param AActor* - �Լ��� ������ Hitted(Hit ���ϴ� ) ��ü
	// FOnHitFunc				OnHitFunc;

	// - Hit Montage ���� ��, CN_EndBeated ���� ���
	//@Warning - Montage �� ���õ� Delegate
	FEndBeatedFunc EndBeatedFunc;

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
	virtual void GetConditionDatasOutArray(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) = 0;

	/* ConditionDatas �� [Index] ���� ������  */
	virtual void GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index) = 0;

	#pragma region Member
public:
	/* HitComp �� �ش��ϴ� ��Ÿ�ֵ��� ���� ���� - ������ ����, ��Ÿ�� �Ұ�, */
	virtual bool IsUsingDamageTypeEffect(uint8 Type) const = 0;

	/* �ٸ� Actor �� ���� �� �ִ��� */
	virtual bool IsCanAttackedFromOther() const = 0;
	virtual void SetCanAttackedFromOther(bool bValue) = 0;

	/* �ٸ� Actor ���� �������� ���� �� �ִ��� */
	virtual bool IsDamagedFromOther() const = 0;
	virtual void SetDamagedFromOther(bool bValue) = 0;

	/* �޺��� ������ ���� - (NotifyState ó��) */
	virtual bool IsCanHittedCombo() const = 0;
	virtual void SetCanHittedCombo(bool bValue) = 0;

public:
	/* Hit ������ ��, ������ '����' */
	FVector GetHitDirection() { return HitDirection; }
	void SetHitDirection(FVector Direction) { HitDirection = Direction; }

	/* Hit ������ ��, ������ '�ӵ�' */
	float GetHitMoveSpeed() const { return HitMoveSpeed; }
	void SetHitMoveSpeed(float fValue) { HitMoveSpeed = fValue; }

protected:
	FVector HitDirection = FVector(0.0f);
	float HitMoveSpeed = 1.0f; //@Charactor Movement �� ���. (Default=1.0f)

	#pragma endregion
};
