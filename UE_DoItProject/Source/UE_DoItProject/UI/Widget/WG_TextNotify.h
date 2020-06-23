#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_TextNotify.generated.h"

UCLASS()
class UE_DOITPROJECT_API UWG_TextNotify 
	: public UUserWidget
{
	GENERATED_BODY()
	
#pragma region Reflection
public:
	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetVisible();

	UFUNCTION(BlueprintCallable, Category = "Function")
		void WigetUnVisible();

#pragma endregion

public:
	UWG_TextNotify(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/* Member */
private:
	class ACPlayer* Player;

};
