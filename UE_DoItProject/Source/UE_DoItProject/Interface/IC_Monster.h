#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "interface/IC_WidgetInfo.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"

#include "IC_Monster.generated.h"


USTRUCT(BlueprintType)
struct FMonsterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
	TArray<FViewConditionState> InfoConditionDataArray;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float MaxHP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float CurrentHP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float Distance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float ATK_Coefficient = 1.0f; //@���� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	float DEF_Coefficient = 1.0f; //@��� ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Value")
	FName Name = "";

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	//float PatrolRadius = 0.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	//float DetectRadius = 0.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	//float AttackRange = 0.0f;

	//# ���� �߰�..
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

	/* Ÿ���� �߰��ϰ� ���ݸ��� ��ȯ AnimInst ���� */
	virtual void SetAIAttackMode(bool bValue) = 0;
	virtual bool GetAIAttackMode() const = 0;

	/* CHumaniodMonster ����� ��. */
//private:
//	bool bAIRunningPossible = true;
};
