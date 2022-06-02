#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CBaseAbility.generated.h"

//@Warning
/* AbilityComponent �� Ŀ�ø� */

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
	/* AbilityComp ���� Ability ���°� '����' �Ǿ��� ��, ����� Delegate */
	FDelStartTimerAbility OnDelStartTimerAbility;

	/* AbilityComp ���� Ability ���°� '����' �Ǿ��� ��, ����� Delegate */
	FEndTimerAbility OnEndTimerAbility;


#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� - TintColor �� UI ������ ȿ�� / ������ ȿ�� ���� */
		FLinearColor TintColor = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
		bool bUsingColorAndOpacity = true;

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

	/* @Warning - Texture �� �������� ���� */
	void Copy(const UCBaseAbility* const In);

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

	/* Ex) ������ ���ݷ��� ������ų ��, Array �� ������. */
	//UI �� ǥ�õǾ��ϱ� ������, �̷� ������ ��Ȳ�� ���� �� �����Ƿ� virtual �� ����
	virtual void TimerRunning(float DeltaTime);

	/* TimeRunning �� ���������� �������� ��Ȳ�� ���ؼ� virtual �� ���� */
	// AbilityComp Tick ���� Timeout �� �����ʱ� ����
	virtual const bool IsTimeOut() const;

	/* Function */
public:
	/* Color Opacity(UI �ð� �ʰ��� ������) ���� ���� */
	void SetUsingColorOpacity(bool fValue) { bUsingColorAndOpacity = fValue; }

protected:
	/* UI Texture Opacity �ʱ�ȭ */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity �� �̿��� �����Ÿ� */
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

	/* �ɷ�ġ�� ����� Actor */
	AActor* AppliedActor;

	// @Target Info UI �� �������� ������ �ð�
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI �� �������� ����
	float OpacityLinearSpeed = 0.01f;
};
