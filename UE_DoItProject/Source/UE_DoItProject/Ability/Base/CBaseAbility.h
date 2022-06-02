#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CBaseAbility.generated.h"

//@Warning
/* AbilityComponent 와 커플링 */

UENUM()
enum class EAbilitySort : uint8
{
	IMMEDITATEAPPLY = 0,
	SAVEARRAY = 1,
};

UENUM()
enum class EAbilityType : int32
{
	NONE		= 0,
	HP			= 1,
	MP			= 2,
	BARRIER		= 3,
	SPEEDUP		= 4,
	SPEEDDOWN	= 5,
	ATKUP		= 6,
	ATKDOWN		= 7,
	DEFUP		= 8,
	DEFDOWN		= 9,
};

USTRUCT()
struct FAbilityValue
{
	GENERATED_USTRUCT_BODY()

public:
	bool bTimer = false;
	EAbilitySort Sort = EAbilitySort::SAVEARRAY; //@Default
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
	/* AbilityComp 에서 Ability 상태가 '시작' 되었을 때, 실행될 Delegate */
	FDelStartTimerAbility OnDelStartTimerAbility;

	/* AbilityComp 에서 Ability 상태가 '종료' 되었을 때, 실행될 Delegate */
	FEndTimerAbility OnEndTimerAbility;


#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 - TintColor 로 UI 긍정적 효과 / 부정적 효과 구분 */
		FLinearColor TintColor = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		bool bUsingColorAndOpacity = true;

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

	/* @Warning - Texture 는 포함하지 않음 */
	void Copy(const UCBaseAbility* const In);

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

	/* Ex) 영구적 공격력을 증가시킬 때, Array 에 들어가야함. */
	//UI 에 표시되야하기 때문에, 이런 예외적 상황이 생길 수 있으므로 virtual 로 구현
	virtual void TimerRunning(float DeltaTime);

	/* TimeRunning 과 마찬가지로 예외적인 상황을 위해서 virtual 로 구현 */
	// AbilityComp Tick 에서 Timeout 이 되지않기 위해
	virtual const bool IsTimeOut() const;

	/* Function */
public:
	/* Color Opacity(UI 시간 초과시 깜빡임) 설정 여부 */
	void SetUsingColorOpacity(bool fValue) { bUsingColorAndOpacity = fValue; }

protected:
	/* UI Texture Opacity 초기화 */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity 를 이용한 깜빡거림 */
	void UpdateUIColorAndOpacity();

	/* Member */
public:
	const EAbilitySort GetAbilitySort() const { return AbilityValue.Sort; }
	const EAbilityType GetAbilityType() const { return AbilityType; }
	const FAbilityValue& GetAbilityValue() const { return AbilityValue; }
	const bool GetUsingTimer() const { return AbilityValue.bTimer; }

	void SetAbilityValue(EAbilitySort Sort, float Val, bool bUsingTimer = false, float TimerVal = 0.0f);
	void SetAbilityValue(const FAbilityValue& Input);
	void SetAppliedActor(AActor* Actor);

	/* Test Code */
	bool IsNullAppliedActor() const { return (AppliedActor != nullptr) ? false : true; }

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
