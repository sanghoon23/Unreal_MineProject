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
	
	#pragma region Reflection
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ConditionValue")
		float test;

public:
	UFUNCTION(BlueprintCallable, Category = "DataValue")
		FText GetInfoMonsterName() const { return FText::FromName(TargetInfo.Name); }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoMonsterHP() const { return TargetInfo.HP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoMonsterDistance() const { return TargetInfo.Distance; }

	UFUNCTION(BlueprintCallable, Category = "ConditionData")
		FLinearColor GetInfoConditionDataLinearColor(int ArrayNumber);

	UFUNCTION(BlueprintCallable, Category = "ConditionData")
		class UConditionData* GetInfoMonsterConditionData(int ArrayNumber);

	UFUNCTION(BlueprintCallable, Category = "ConditionData")
		TArray<class UConditionData*> GetInfoMonsterConditionDataArray();

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
