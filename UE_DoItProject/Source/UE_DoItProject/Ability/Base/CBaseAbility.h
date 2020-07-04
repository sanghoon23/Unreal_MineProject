#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CBaseAbility.generated.h"

//@Warning
/* AbilityComponent 와 커플링 */

UENUM()
enum class EAbilityType : int32
{
	NONE		= 0,
	HP			= 1,
	MP			= 2,
	BARRIER		= 3,
	SPEEDUP		= 4,
	SPEEDDOWN	= 4,
	ATK			= 5,
	DEF			= 6,
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

DECLARE_MULTICAST_DELEGATE_OneParam(FDelStartTimerAbility, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FEndTimerAbility, AActor*)

UCLASS()
class UE_DOITPROJECT_API UCBaseAbility 
	: public UObject
{
	GENERATED_BODY()

public:
	/* AbilityComp 에서 Ability 상태가 시작되었을 때, 실행될 Delegate */
	FDelStartTimerAbility OnDelStartTimerAbility;

	/* AbilityComp 에서 Ability 상태가 종료 되었을 때, 실행될 Delegate */
	FEndTimerAbility OnEndTimerAbility;


#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo 표시 Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* @true - LeftToRight, @false - RightToLeft*/
		bool bLinerColorDir = true;

private:

#pragma endregion

public:
	UCBaseAbility();

	/* Pure Viratul Fucntion - IWidget */
public:
	class UTexture2D* GetTextureUI() { return TextureUI; }
	float GetApplyTimer() { return AbilityValue.Timer; }

	/* Virtual Function */
public:
	/* Timer 를 사용한 능력치 적용 시작 */
	virtual void StartUseTimerAbility();

	/* Timer 를 사용할 때 AbilityComponent 에서 Tick */
	virtual void TickUseTimerAbility(float DeltaTime);

	/* Timer 를 사용한 능력치 적용 종료 */
	virtual void EndUseTimerAbility();

	/* 
	Timer 를 사용한 능력치 적용 에서,
	AbilityComponent TMap 에 중첩되어서 들어갈 때,

	@param OverlapAbility - Add될 Ability
	@return - Overlap(중첩) 되었는지 여부
	*/
	virtual void OverlapAbility(class UCBaseAbility* Ability);

	/* Timer 를 사용하지 않고 영구적으로 능력치 적용 */
	virtual void ApplyAbility() {}

	/* Function */
public:
	void TimerRunning(float DeltaTime);

protected:
	/* UI Texture Opacity 초기화 */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity 를 이용한 깜빡거림 */
	void UpdateUIColorAndOpacity();

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

	// @Target Info UI 에 깜빡임을 시작할 시간
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI 에 깜빡임의 정도
	float OpacityLinearSpeed = 0.01f;
};
