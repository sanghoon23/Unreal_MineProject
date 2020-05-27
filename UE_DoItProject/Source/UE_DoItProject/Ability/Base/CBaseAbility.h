#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBaseAbility.generated.h"

//@Warning
/* AbilityComponent �� Ŀ�ø� */

UENUM()
enum class EAbilityType : int32
{
	NONE	= 0,
	HP		= 1,
	MP		= 2,
	BARRIER	= 3,
	SPEED	= 4,
	ATK		= 5,
	DEF		= 6,
};

USTRUCT()
struct FAbilityValue
{
	GENERATED_USTRUCT_BODY()

public:
	bool bTimer = false;
	float Timer = 0.0f;

	float Value = 0.0f;
};

UCLASS()
class UE_DOITPROJECT_API UCBaseAbility 
	: public UObject
{
	GENERATED_BODY()
	
public:
	UCBaseAbility();

	/* Virtual Function */
public:
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility() {}

	/* Timer �� ����� �� AbilityComponent ���� Tick */
	virtual void TickUseTimerAbility() {}

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility() {}

	/* 
	Timer �� ����� �ɷ�ġ ���� ����,
	AbilityComponent TMap �� ��ø�Ǿ �� ��,

	@param OverlapAbility - Add�� Ability
	@return - Overlap(��ø) �Ǿ����� ����
	*/
	virtual bool OverlapAbility(class UCBaseAbility* Ability) { return false; }

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	virtual void ApplyAbility() {}

	/* Function */
public:
	void TimerRunning(float DeltaTime) { AbilityValue.Timer -= DeltaTime; }

	/* Member */
public:
	const EAbilityType GetAbilityType() const { return AbilityType; }
	const FAbilityValue& GetAbilityValue() const { return AbilityValue; }
	const bool GetUsingTimer() const { return AbilityValue.bTimer; }
	const bool IsTimeOut() const;

	void SetAbilityValue(float Val, bool bUsingTimer = false, float TimerVal = 0.0f);
	void SetAbilityValue(const FAbilityValue& Input);
	void SetAppliedActor(AActor* Actor);

protected:
	EAbilityType AbilityType;

	FAbilityValue AbilityValue;

	/* �ɷ�ġ�� ����� Actor */
	AActor* AppliedActor;
};
