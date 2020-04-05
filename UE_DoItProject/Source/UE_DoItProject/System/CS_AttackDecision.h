#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CS_AttackDecision.generated.h"

/* 자동 공격 사용 여부 ENUM */
UENUM()
enum class EAutoAttackable : uint8
{
	UN_USE = 0,
	USE = 1,
};

UCLASS()
class UE_DOITPROJECT_API UCS_AttackDecision 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	UCS_AttackDecision();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	void OnAble(ACharacter* OwnerCharactor, float AtkRange);
	void UnAble();

	void StartAttackTrace(AActor* TraceTarget);
	void StopAttackTrace();

	#pragma region Member
	/* Member */
public:
	EAutoAttackable GetAble() const { return Attackable; }
	bool GetDecisionPossible() const { return bAttackPossible; }

private:
	EAutoAttackable Attackable = EAutoAttackable::UN_USE;
	ACharacter* SettedCharactor;
	
	AActor* Target = nullptr;
	bool bAutoAttackCall = false;
	bool bAttackPossible = true;

	float AttackRange = 0.0f;

	#pragma endregion
};
