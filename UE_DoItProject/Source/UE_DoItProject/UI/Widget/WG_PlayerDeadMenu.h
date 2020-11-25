#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_PlayerDeadMenu.generated.h"

UCLASS()
class UE_DOITPROJECT_API UWG_PlayerDeadMenu 
	: public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "Func")
		void RespawnFunc();

	UFUNCTION(BlueprintCallable, Category = "Func")
		void SpeactatorModeFunc();

private:
	UPROPERTY(VisibleAnywhere, Category = "Sequence")
		class ULevelSequence* FadeOutDisplay;

	UPROPERTY(VisibleAnywhere, Category = "Sequence")
		class ULevelSequencePlayer* LevelSequencePlayer;


#pragma endregion

public:
	UWG_PlayerDeadMenu(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void StartingDeadMenu();

public:
	void WigetVisible();
	void WigetUnVisible();

private:
	class ACPlayer* Player;

};
