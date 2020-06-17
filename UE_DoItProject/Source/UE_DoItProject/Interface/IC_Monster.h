#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_Charactor.h"
#include "Interface//IC_HitComp.h"
#include "DamagedConditionType/Base/CBaseConditionType.h"

#include "IC_Monster.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset")
	TArray<UCBaseConditionType*> InfoConditionDataArray;

public:
	float MaxHP = 0.0f;
	float CurrentHP = 0.0f;
	float Distance = 0.0f;
	FName Name = "";

	//# 상태 추가..
	//class 
};

UINTERFACE(MinimalAPI)
class UIC_Monster : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_Monster
{
	GENERATED_BODY()

	/* Pure Virtual Function */
public:
	virtual AActor* GetTargetInAI() = 0;

public:
	virtual const FMonsterInfo& GetMonsterInfo() const = 0;

	virtual void SetAIRunningPossible(bool bValue) = 0;
	virtual bool GetAIRunningPossible() const = 0;

	/* 타겟을 발견하고 공격모드로 전환 AnimInst 연동 */
	virtual void SetAIAttackMode(bool bValue) = 0;
	virtual bool GetAIAttackMode() const = 0;

	/* CHumaniodMonster 멤버로 뺌. */
//private:
//	bool bAIRunningPossible = true;
};
