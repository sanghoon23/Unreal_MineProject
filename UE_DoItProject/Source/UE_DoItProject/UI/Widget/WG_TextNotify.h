#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_TextNotify.generated.h"

USTRUCT()
struct FUITextData
{
	GENERATED_USTRUCT_BODY()

public:
	float Timer = 0.0f;
	float ColorOpacity = 1.0f;
	FString CurrentText;
};

UCLASS()
class UE_DOITPROJECT_API UWG_TextNotify 
	: public UUserWidget
{
	GENERATED_BODY()

	const float VisibleTime = 3.0f;

#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetVisible();

	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetUnVisible();

public:
	UFUNCTION(BlueprintCallable, Category = "Function")
		/* 현재 Text */
		FText GetCurrentText();

	UFUNCTION(BlueprintCallable, Category = "Function")
		float GetCurrentTextTimer() const { return CurrentData.Timer; }

	UFUNCTION(BlueprintCallable, Category = "Function")
		float GetColorOpacity() const { return CurrentData.ColorOpacity; }

	UFUNCTION(BlueprintCallable, Category = "Function")
		/* UI 가 지금 사용 중인지 */
		bool IsUsingNotify() const { return bTickTimer; }

#pragma endregion

	//TODO : Opacity
public:
	void CallingUITextNotify(const FString& InputText, float fTime = 0.0f);

private:
	void InitUITextData();

public:
	UWG_TextNotify(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Member */
private:
	class ACPlayer* Player;

	bool bTickTimer = false;
	FUITextData CurrentData;
	float Timer = 0.0f;

};
