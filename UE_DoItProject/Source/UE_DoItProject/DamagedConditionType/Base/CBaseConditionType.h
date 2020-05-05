#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CBaseConditionType.generated.h"

UENUM()
/* 상태 이상 종류 */
enum class EHitUpset : uint8
{
	SLOWMOVE	= 0, //이동감속
	STUN		= 1, //스턴
	BURN		= 2, //화상
	POISION		= 3, //중독
	FREEZE		= 4, //빙결
	SLEEP		= 5, //수면
};

UCLASS(BlueprintType, ClassGroup = (Custom))
class UE_DOITPROJECT_API UCBaseConditionType 
	: public UObject
{
	GENERATED_BODY()
public:
	UCBaseConditionType() {};

	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* @true - LeftToRight, @false - RightToLeft*/
		bool bLinerColorDir = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo 표시 Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		/* 상태 적용 시간 */
		float ApplyTime = 0.0f;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Data")
		EHitUpset State;

	#pragma endregion

///////////////////////////////////////////////////////////////////////////////////////////////////


	/* Virtual Function */
public:
	/* 해당 Owner 의 HitComp AddCondition 될 때 실행 함수*/
	virtual void StartConditionOnActor(AActor* Owner) {};

	/* 해당 Owner 의 HitComp Tick 에서 Update */
	virtual void UpdateConditionOnActor(AActor* Owner, float DeltaTime);

	/* 해당 Onwer 의 HitComp 에서 ApplyTime 이 끝난 후, 실행 함수 */
	virtual void EndConditionOnActor(AActor * Owner);

	/* 해당 상태가 중첩되었을 때, */
	virtual void ConditionOverlap(UCBaseConditionType* OverlappedCondition) {}

protected:
	/* UI Texture Opacity 초기화 */
	void InitUIColorAndOpacity();

	/* UI Texture Opacity 를 이용한 깜빡거림 */
	void UpdateUIColorAndOpacity(UCBaseConditionType* ConditionData);

	/* Member */
public:
	EHitUpset GetState() const { return State; }
	void SetState(EHitUpset Type) { State = Type; }

protected:
	// @Target Info UI 에 깜빡임을 시작할 시간
	float OpacityLinearTimer = 3.0f;

	// @Target Info UI 에 깜빡임의 정도
	float OpacityLinearSpeed = 0.01f;
};
