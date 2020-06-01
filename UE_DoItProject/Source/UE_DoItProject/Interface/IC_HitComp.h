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
	// - Hit 됐을 때, 실행할 함수 Delegate
	// @param AActor* - 함수를 수행할 Hitted(Hit 당하는 ) 객체
	// FOnHitFunc				OnHitFunc;

	// - Hit 됐을 때, 기존 상태값들을 원래로 되돌리는 Delegate
	// @param AActor* - Hitted( Hit 당하는 ) 객체
	// FOnHitResetState		OnHitResetState;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Pure Virtual Function */
public:
	// @AttackingActor - 때리는 주체 Actor
	// @DamageType - 데미지 유형 (Ex. Normal, Air...) 등
	// @DamageAmount - 데미지량
	virtual void OnHit(AActor* AttackingActor, UCDamageType_Base* const DamageType, float DamageAmount) = 0;

	/* 대상이 맞고 있는지 */
	virtual const bool IsBeated() const = 0;

	/* 대상이 맞고 있는지 설정 - CN_BeginBeat */
	virtual void SetBeated(bool bValue) = 0;

	/* HitComponent 안의 상태이상, 행동불가 데이터 */
	// @ArrayNumber - 배열 컨테이너의 Index
	virtual UCBaseConditionType* GetConditionData(int Index) = 0;
	
	/* OutDataArray->Empty(갱신) 후 ConditionDatas 모든 데이터를 OutDataArray->'Push' */
	//@param - OutDataArray (OUT)
	//@parma Count - 가져올 갯수 (Default -1, 모두 가져옴)
	virtual void GetConditionDatasAfterEmpty(TArray<UCBaseConditionType*>* OutDataArray, int Count = -1) = 0;

	/* ConditionDatas 의 [Index] 값을 가져옴  */
	virtual void GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index) = 0;

	/* ConditionDatas 를 Count(범위) 까지 가져옴 */
	//@param OutDataArray - OUT
	//@param Count - 가져올 갯수
	virtual void GetConditionDatasByCount(TArray<UCBaseConditionType*>* OutDataArray, int IndexCount) = 0;

	#pragma region Member
public:
	virtual void SetBlockDamagedMontage(bool bValue) = 0;

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
