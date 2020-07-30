#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_FloatingCombo.generated.h"

UENUM()
enum class EFloatingComboColor //@데미지 주체자 에 따른 색 변화
{
	WHITE	= 0,
	RED		= 1,
};

UCLASS()
class UE_DOITPROJECT_API UWG_FloatingCombo 
	: public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "Data")
		FVector2D GetInitialLocation() const { return Location; }

	UFUNCTION(BlueprintCallable, Category = "Data")
		FSlateColor GetInitialColor() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
		float GetDisplayDamageValue() const { return DisplayDamageValue; }


#pragma endregion
public:
	UWG_FloatingCombo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Function */
public:
	void SetInitial(APlayerController* PC, FVector WorldLocation, EFloatingComboColor UIColor);
	void SetDisplayDamageValue(float fValue) { DisplayDamageValue = fValue; }

	/* Member */
private:
	FVector2D Location = FVector2D(0.0f);
	FLinearColor Color = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));

	float DisplayDamageValue = 0.0f;
};
