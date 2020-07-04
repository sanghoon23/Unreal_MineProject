#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "IC_WidgetInfo.generated.h"

USTRUCT(BlueprintType)
struct FViewConditionState
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* UI - TargetInfo 표시 Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* 상태정보 UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		/* 상태 적용 시간 */
		float ApplyTime = 0.0f;
};

UINTERFACE(MinimalAPI)
class UIC_WidgetInfo : public UInterface
{
	GENERATED_BODY()
};

class UE_DOITPROJECT_API IIC_WidgetInfo
{
	GENERATED_BODY()
	
	/* Pure Virtual Function */
public:
	/* 캐릭터 Info UI 에 표시될 ConditionState */
	virtual void GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray) = 0;

};
