#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_SkillCastingBar.generated.h"

UCLASS()
class UE_DOITPROJECT_API UWG_SkillCastingBar 
	: public UUserWidget
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
		float FillSpeed = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Value")
		bool GetFinishFilledProgressBar() const { return bFinishFilledProgress; }

	UFUNCTION(BlueprintCallable, Category = "Value")
		float GetSkillCastingPersentage() const { return Persentage; }

	UFUNCTION(BlueprintCallable, Category = "Value")
		/* 
		@param - FillingSpeed ( 채워지는 속도 )
		@param - StartPercentage ( 시작될 Percentage ) Default = 0.0f;
		@parma - Reverse ( false - LeftToRight, true - RightToLeft ) Default = false;
		*/
		void StartProgress(float FillingSpeed, float StartPercentage = 0.0f, bool Reverse = false);

	UFUNCTION(BlueprintCallable, Category = "Value")
		void EndProgress();

	#pragma endregion

public:
	UWG_SkillCastingBar(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Function */
public:

	/* Member */
private:
	bool bPlaying = false;

	float Persentage	= 0.0f;
	bool bReverse = false;

	bool bFinishFilledProgress = false;
};
