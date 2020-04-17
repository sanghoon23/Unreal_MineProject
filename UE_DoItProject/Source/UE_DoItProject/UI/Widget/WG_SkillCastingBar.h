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

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Value")
	//	float MaxRange = 100.0f;

	UFUNCTION(BlueprintCallable, Category = "Value")
		bool GetFinishFilledProgressBar() const { return bFinishFilledProgress; }

	UFUNCTION(BlueprintCallable, Category = "Value")
		float GetSkillCastingPersentage() const { return Persentage; }

	UFUNCTION(BlueprintCallable, Category = "Value")
		void StartProgress();

	UFUNCTION(BlueprintCallable, Category = "Value")
		void EndProgress();

	//UFUNCTION(BlueprintCallable, Category = "Value")
	//	void ProgressAction();

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

	bool bFinishFilledProgress = false;
};
