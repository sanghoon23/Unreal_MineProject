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
	float HP = 0.0f;
	float Distance = 0.0f;
	FName Name = "";

	//# »óÅÂ Ãß°¡..
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
	virtual void SetAIRunningPossible(bool bValue) = 0;
	virtual bool GetAIRunningPossible() const = 0;

	/* CHumaniodMonster ¸â¹ö·Î »­. */
//private:
//	bool bAIRunningPossible = true;
};
