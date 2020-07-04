#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CBaseAbility.generated.h"

//@Warning
/* AbilityComponent �� Ŀ�ø� */

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
	/* AbilityComp ���� Ability ���°� ���۵Ǿ��� ��, ����� Delegate */
	FDelStartTimerAbility OnDelStartTimerAbility;

	/* AbilityComp ���� Ability ���°� ���� �Ǿ��� ��, ����� Delegate */
	FEndTimerAbility OnEndTimerAbility;


#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo ǥ�� Texture */
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
	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void StartUseTimerAbility();

	/* Timer �� ����� �� AbilityComponent ���� Tick */
	virtual void TickUseTimerAbility(float DeltaTime);

	/* Timer �� ����� �ɷ�ġ ���� ���� */
	virtual void EndUseTimerAbility();

	/* 
	Timer �� ����� �ɷ�ġ ���� ����,
	AbilityComponent TMap �� ��ø�Ǿ �� ��,

	@param OverlapAbility - Add�� Ability
	@return - Overlap(��ø) �Ǿ����� ����
	*/
	virtual void OverlapAbility(class UCBaseAbility* Ability);

	/* Timer �� ������� �ʰ� ���������� �ɷ�ġ ���� */
	virtual void ApplyAbility() {}

	/* Function */
public:
	void TimerRunning(float DeltaTime);

protected:
	/* UI Texture Opacity �ʱ�ȭ */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity �� �̿��� �����Ÿ� */
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

	/* �ɷ�ġ�� ����� Actor */
	AActor* AppliedActor;

	// @Target Info UI �� �������� ������ �ð�
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI �� �������� ����
	float OpacityLinearSpeed = 0.01f;
};
