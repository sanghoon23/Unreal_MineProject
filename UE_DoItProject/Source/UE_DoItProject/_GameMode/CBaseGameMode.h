#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"

#include "CBaseGameMode.generated.h"

UENUM()
enum class EMyGamePlayState
{
	EPlaying	= 0,
	EGameOver	= 1,
	EUnknown	= 2,
};

UCLASS()
class UE_DOITPROJECT_API ACBaseGameMode 
	: public AGameModeBase
{
	GENERATED_BODY()
	
	#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "GameState")
		EMyGamePlayState GetMyGamePlayState() const;

	//UFUNCTION(BlueprintCallable, Category = "Widget")
	//	void SetCurrentWidget(TSubclassOf<UUserWidget> InputWidget);

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
	//	TSubclassOf<UUserWidget> StartWidgetClass;

	private:

	#pragma endregion

public:
	ACBaseGameMode();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/* Function */
public:
	void SettingGamePlayState(EMyGamePlayState MyGameState) { CurrentMyGamePlayState = MyGameState; }

	/* Member */
public:

private:
	/* 현재 게임 상태 */
	EMyGamePlayState CurrentMyGamePlayState = EMyGamePlayState::EPlaying;

	/* 현재 위젯 */
	//UUserWidget* CurrentWidget;
};
