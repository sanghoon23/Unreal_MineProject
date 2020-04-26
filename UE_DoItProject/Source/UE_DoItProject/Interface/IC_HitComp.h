#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Color.h"

#include "DamageType/Base/CDamageType_Base.h"

#include "IC_HitComp.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @ConditionData Class

/* Base Struct - Blueprint ���� ��� */
UCLASS(BlueprintType, ClassGroup = (Custom))
class UE_DOITPROJECT_API UConditionData
	: public UObject
{
	GENERATED_BODY()
public:
	UConditionData() {};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	/* �������� UI Color&Opacity */
	FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	/* @true - LeftToRight, @false - RightToLeft*/
	bool bLinerColorDir = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	/* ���� ���� �ð� */
	float ApplyTime = 0.0f;
};

/* �����̻� - Ex) �ߵ�, ȭ�� �� */
UCLASS(BlueprintType, ClassGroup = (Custom))
class UE_DOITPROJECT_API UHitUpsetConditionData
	: public UConditionData
{
	GENERATED_BODY()
public:
	UHitUpsetConditionData() {};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Montage")
		class UAnimMontage* UpsetConditionMon = nullptr;
};

/* �ൿ �Ұ� - Ex) ����, ���� �� */
UCLASS(BlueprintType, ClassGroup = (Custom))
class UE_DOITPROJECT_API UHitNonActionConditionData
	: public UConditionData
{
	GENERATED_BODY()

public:
	UHitNonActionConditionData() {};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Montage")
		class UAnimMontage* NonActionMon = nullptr;
};

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
	/* Virtual Function */
public:
	// @AttackingActor - ������ ��ü Actor
	// @DamageType - ������ ���� (Ex. Normal, Air...) ��
	// @DamageAmount - ��������
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base* const DamageType, float DamageAmount) = 0;

	/* HitComponent ���� �����̻�, �ൿ�Ұ� ������ */
	// @ArrayNumber - �迭 �����̳��� Index
	virtual UConditionData* GetConditionData(int Index) = 0;
	virtual void GetConditionDatasAfterEmpty(TArray<UConditionData*>* OutDataArray) = 0;
	virtual void GetConditionDatasWithIndex(TArray<UConditionData*>* OutDataArray, int Index) = 0;

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
