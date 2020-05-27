#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interface/IC_Player.h"

#include "WG_PlayerInfo.generated.h"

UCLASS()
class UE_DOITPROJECT_API UWG_PlayerInfo 
	: public UUserWidget
{
	GENERATED_BODY()
	
private:
	const uint8 ConditionUITextureNumber = 5;

	#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "DataValue")
		FText GetInfoPlayerName() const { return FText::FromName(PlayerInfo.Name); }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoPlayerMaxHP() const { return PlayerInfo.MaxHP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoPlayerCurrentHP() const { return PlayerInfo.CurrentHP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoPlayerMaxMP() const { return PlayerInfo.MaxMP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoPlayerCurrentMP() const { return PlayerInfo.CurrentMP; }

	UFUNCTION(BlueprintCallable, Category = "DataValue")
		float GetInfoPlayerBarrierAmount() const { return PlayerInfo.BarrierAmount; }

public:
	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetVisible();

	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetUnVisible();

	#pragma endregion

public:
	UWG_PlayerInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Member */
private:
	void InitPlayerInfo();
	void InsertPlayerInfo(const FPlayerInfo& Insert);

private:
	class ACPlayer* Player;

	FPlayerInfo PlayerInfo;

};
