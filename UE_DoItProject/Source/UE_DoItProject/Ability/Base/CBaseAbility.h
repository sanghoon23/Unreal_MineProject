#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBaseAbility.generated.h"

//@Warning
/* AbilityComponent 와 커플링 */

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
	/* Timer 를 사용한 능력치 적용 시작 */
	virtual void StartUseTimerAbility() {}

	/* Timer 를 사용할 때 AbilityComponent 에서 Tick */
	virtual void TickUseTimerAbility() {}

	/* Timer 를 사용한 능력치 적용 종료 */
	virtual void EndUseTimerAbility() {}

	/* 
	Timer 를 사용한 능력치 적용 에서,
	AbilityComponent TMap 에 중첩되어서 들어갈 때,

	@param OverlapAbility - Add될 Ability
	@return - Overlap(중첩) 되었는지 여부
	*/
	virtual bool OverlapAbility(class UCBaseAbility* Ability) { return false; }

	/* Timer 를 사용하지 않고 영구적으로 능력치 적용 */
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

	/* 능력치가 적용될 Actor */
	AActor* AppliedActor;
};
