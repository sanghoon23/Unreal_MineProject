#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Math/Color.h"

#include "DamageType/Base/CDamageType_Base.h"

#include "IC_HitComp.generated.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @ConditionData Class

/* Base Struct - Blueprint 에서 사용 */
UCLASS(BlueprintType, ClassGroup = (Custom))
class UE_DOITPROJECT_API UConditionData
	: public UObject
{
	GENERATED_BODY()
public:
	UConditionData() {};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	/* 상태정보 UI Color&Opacity */
	FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	/* @true - LeftToRight, @false - RightToLeft*/
	bool bLinerColorDir = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	/* 상태 적용 시간 */
	float ApplyTime = 0.0f;
};

/* 상태이상 - Ex) 중독, 화상 등 */
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

/* 행동 불가 - Ex) 기절, 빙결 등 */
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
	// - Hit 됐을 때, 실행할 함수 Delegate
	// @param AActor* - 함수를 수행할 Hitted(Hit 당하는 ) 객체
	// FOnHitFunc				OnHitFunc;

	// - Hit 됐을 때, 기존 상태값들을 원래로 되돌리는 Delegate
	// @param AActor* - Hitted( Hit 당하는 ) 객체
	// FOnHitResetState		OnHitResetState;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Virtual Function */
public:
	// @AttackingActor - 때리는 주체 Actor
	// @DamageType - 데미지 유형 (Ex. Normal, Air...) 등
	// @DamageAmount - 데미지량
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base* const DamageType, float DamageAmount) = 0;

	/* HitComponent 안의 상태이상, 행동불가 데이터 */
	// @ArrayNumber - 배열 컨테이너의 Index
	virtual UConditionData* GetConditionData(int Index) = 0;
	virtual void GetConditionDatasAfterEmpty(TArray<UConditionData*>* OutDataArray) = 0;
	virtual void GetConditionDatasWithIndex(TArray<UConditionData*>* OutDataArray, int Index) = 0;

	#pragma region Member
public:
	/* Hit 당했을 때, 움직일 '방향' */
	FVector GetHitDirection() { return HitDirection; }

	/* Hit 당했을 때, 움직일 '방향' */
	void SetHitDirection(FVector Direction) { HitDirection = Direction; }

	/* Hit 당했을 때, 움직일 '속도' */
	float GetHitMoveSpeed() const { return HitMoveSpeed; }

	/* Hit 당했을 때, 움직일 '속도' */
	void SetHitMoveSpeed(float fValue) { HitMoveSpeed = fValue; }

protected:
	FVector HitDirection = FVector(0.0f);
	float HitMoveSpeed = 1.0f; //@Charactor Movement 를 사용. (Default=1.0f)

	#pragma endregion
};
