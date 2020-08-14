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
		/* UI - TargetInfo ǥ�� Texture */
		class UTexture2D* TextureUI = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� UI Color&Opacity */
		FLinearColor ColorAndOpacity = FLinearColor(FVector4(1.0f));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
		/* �������� - TintColor �� UI ������ ȿ�� / ������ ȿ�� ���� */
		FSlateColor TintSlateColor = FSlateColor(FLinearColor(FVector4(1.0f)));

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		/* ���� ���� �ð� */
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
	/* ĳ���� Info UI �� ǥ�õ� ConditionState */
	//@param OutArray - ȭ�鿡 ǥ�õ� ���µ����͵��� FViewConditionState �� ��ȯ�Ǿ� �Ѿ
	//ex) TargetInfo, PlayerInfo
	virtual void GetViewConditionStateForUI(TArray<FViewConditionState>* OutArray) = 0;

	//@������ ǥ�� �� �ѹ� �ߴ�
	//- �Ͻ������� �ѹ��� �ߴ� Ex) HumanoidMonster,Player TakeDamage ���� �ٽ� true �� ��ȯ��.
	void SetOnceNoneUsingFloatingCombo() { bUsingFloatingComboUI = false; }

	/* Member */
protected:
	//@FloatingCombo �ڵ� On ����ġ
	bool bUsingFloatingComboUI = true;

};
