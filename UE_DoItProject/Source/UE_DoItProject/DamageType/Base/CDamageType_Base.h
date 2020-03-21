#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "CDamageType_Base.generated.h"

UENUM()
enum class FConditionType
{
	NORMAL = 0,
	AIR = 1,
	AIRATTACK = 2,
	STRONGATTACK = 3,
	STUN = 4,
};

DECLARE_MULTICAST_DELEGATE_OneParam(FDamageFunc, AActor*)

UCLASS()
class UE_DOITPROJECT_API UCDamageType_Base 
	: public UDamageType
{
	GENERATED_BODY()
	
	#pragma region Reflection
protected:
	UPROPERTY(EditAnywhere, Category = "DamageType")
		int TypeNumber = 0;

	UPROPERTY(EditAnywhere, Category = "DamageType")
		FConditionType ConditionType;

	#pragma endregion

public:
	UCDamageType_Base();

	/* Virtual */
public:
	// @DamageFunc Delegate 수행 
	// @param Actor - 공격당하는, 데미지 받는 Actor
	virtual void OnDamageDelegate(AActor* DamagedActor);

	#pragma region Member
public:
	FDamageFunc& GetDamageDelegate() { return DamageFunc; }

	int GetTypeNumber() const { return TypeNumber; }
	FConditionType GetConditionType() const { return ConditionType; }

protected:
	/* @HitComp 에 들어갔을 때, 수행할 Delegate */
	FDamageFunc DamageFunc;

	#pragma endregion
};
