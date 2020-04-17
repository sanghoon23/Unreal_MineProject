#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/WG_SkillCastingBar.h"
#include "UI/Widget/WG_TargetInfo.h"
#include "UI/Widget/WG_PlayerInfo.h"

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
	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<class UUserWidget> TargetInfoWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Casting")
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
	UWG_SkillCastingBar* CastingWidget;

	UWG_TargetInfo*		TargetInfoWidget;
	
	//Test Code
	UUserWidget* InputWidget;
};
