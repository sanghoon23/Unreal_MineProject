#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/IC_Monster.h"

#include "WG_TargetInfo.generated.h"

UCLASS()
class UE_DOITPROJECT_API UWG_TargetInfo 
	: public UUserWidget
{
	GENERATED_BODY()
		
private:
	const uint8 ConditionUITextureNumber = 5;
	

	#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "DataValue")
		FText GetInfoMonsterName() const { return FText::FromName(TargetInfo.Name); }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoMonsterMaxHP() const { return TargetInfo.MaxHP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoMonsterCurrentHP() const { return TargetInfo.CurrentHP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoMonsterDistance() const { return TargetInfo.Distance; }

	UFUNCTION(BlueprintCallable, Category = "ConditionData")
		class UTexture2D* GetInfoConditionTextureUI(int ArrayNumber);

	UFUNCTION(BlueprintCallable, Category = "ConditionData")
		FLinearColor GetInfoConditionDataLinearColor(int ArrayNumber);

	//UFUNCTION(BlueprintCallable, Category = "ConditionData")
	//	class UCBaseConditionType* GetInfoMonsterConditionData(int ArrayNumber);

	//UFUNCTION(BlueprintCallable, Category = "ConditionData")
	//	TArray<class UCBaseConditionType*> GetInfoMonsterConditionDataArray();

	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetVisible();

	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetUnVisible();

	#pragma endregion

public:
	UWG_TargetInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Member */
private:
	void InitTargetInfo();

private:
	class ACPlayer* Player;

	FMonsterInfo TargetInfo;
};
