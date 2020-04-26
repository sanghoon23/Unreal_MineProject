#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/IC_Charactor.h"
#include "Interface//IC_HitComp.h"

#include "IC_Monster.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Asset")
	TArray<UConditionData*> InfoConditionDataArray;

public:
	float HP = 0.0f;
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
	virtual FMonsterInfo GetMonsterInfo() const = 0;

	/* Member */
public:
	void SetAIRunningPossible(bool bValue) { bAIRunningPossible = bValue; }
	bool GetAIRunningPossible() const { return bAIRunningPossible; }

private:
	bool bAIRunningPossible = true;
};
