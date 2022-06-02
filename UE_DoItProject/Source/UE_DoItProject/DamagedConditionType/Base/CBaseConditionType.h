#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


#include "CBaseConditionType.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FDelStartCondition, AActor*)
DECLARE_MULTICAST_DELEGATE_OneParam(FDelEndCondition, AActor*)

UENUM()
/* 상태 이상 종류 */
enum class EHitUpset : uint8
{
	NONE		= 0,
	SLOWER		= 1, //이동감속
	STUN		= 2, //스턴
	BURN		= 3, //화상
	POISION		= 4, //중독
	FREEZE		= 5, //빙결
	SLEEP		= 6, //수면
};

UCLASS()
class UE_DOITPROJECT_API UCBaseConditionType 
	: public UObject
{
	GENERATED_BODY()

public:
	/* HitComp 에서 Condition 상황이 '시작' 되었을 때, 실행될 Delegate */
	FDelStartCondition OnDelStartCondition;

	/* HitComp 에서 Condition 상황이 '종료' 되었을 때, 실행될 Delegate */
	FDelEndCondition OnDelEndCondition;

	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 - TintColor 로 UI 긍정적 효과 / 부정적 효과 구분 */
		FLinearColor TintColor = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		/* 상태 적용 시간 */
		float ApplyTime = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo 표시 Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* @true - LeftToRight, @false - RightToLeft*/
		bool bLinerColorDir = true;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		EHitUpset State;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		FDamageEvent DamageEvent;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		class AController* DamageSubjectController;

	#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////

public:
	UCBaseConditionType();

	/* @Warning - Texture 는 포함하지 않음 */
	void Copy(const UCBaseConditionType* const In);


	/* Virtual Function */
public:
	/* 해당 Owner 의 HitComp AddCondition 될 때 실행 함수 */
	virtual void StartCondition(APawn* Owner);

	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateCondition(APawn* Owner, float DeltaTime);

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행 함수 */
	virtual void EndCondition(APawn* Owner);

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Function */
public:
	class UTexture2D* GetTextureUI() { return TextureUI; }
	void SetTextureUI(class UTexture2D* Texture);

	/* ConditionType Damage 를 일으킨 주체자 설정 */
	void SetCauser(AActor* InCauser) { Causer = InCauser; }

protected:
	/* UI Texture Opacity 초기화 */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity 를 이용한 깜빡거림 */
	void UpdateUIColorAndOpacity(UCBaseConditionType* ConditionData);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* Member */
public:
	EHitUpset GetState() const { return State; }
	void SetState(EHitUpset Type) { State = Type; }

	const FDamageEvent& GetDamageEvent() const { return DamageEvent; }
	void SetDamageEvent(const FDamageEvent& Event) { DamageEvent = Event; }

	class AController* GetDamageSubjectController() { return DamageSubjectController; }
	void SetDamageSubjectController(class AController* InputController);

	float GetApplyTime() const { return ApplyTime; }

protected:
	// @Target Info UI 에 깜빡임을 시작할 시간
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI 에 깜빡임의 정도
	float OpacityLinearSpeed = 0.01f;

	//@Damage 를 준 Causer (주체자)
	AActor* Causer;
};
