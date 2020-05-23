#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/WG_PlayerInfo.h"
#include "UI/Widget/WG_TargetInfo.h"
#include "UI/Widget/WG_SkillCastingBar.h"

#include "HUD_Main.generated.h"

UCLASS()
class UE_DOITPROJECT_API AHUD_Main 
	: public AHUD
{
	GENERATED_BODY()

	#pragma region Reflection

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
		void SetInputWidget(TSubclassOf<UUserWidget> InputWidget);

private:
	UPROPERTY(EditAnywhere, Category = "Visible")
		TSubclassOf<class UUserWidget> PlayerInfoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Visible")
		TSubclassOf<class UUserWidget> TargetInfoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Visible")
		TSubclassOf<class UUserWidget> CastingWidgetClass;

	//UPROPERTY(EditAnywhere, Category = "Health")
	//	class UUserWidget* HealthWidget;

	#pragma endregion	

public:
	AHUD_Main();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;


	/* Memeber */
public:
	UWG_SkillCastingBar* GetWidgetSkillCastingBar() { return CastingWidget; }
	UWG_TargetInfo* GetWidgetTargetInfo() { return TargetInfoWidget; }

private:
	UWG_PlayerInfo*			PlayerInfoWidget;

	UWG_TargetInfo*			TargetInfoWidget;
	
	UWG_SkillCastingBar*	CastingWidget;

	//Test Code
	UUserWidget* InputWidget;
};
