#include "WG_FloatingCombo.h"
#include "CLog.h"
#include "UMG.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Character.h"

UWG_FloatingCombo::UWG_FloatingCombo(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWG_FloatingCombo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWG_FloatingCombo::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UWG_FloatingCombo::SetInitial(APlayerController* PC, FVector WorldLocation, EFloatingComboColor UIColor)
{
	check(PC);

	//@Setting - Random Range
	{
		PC->ProjectWorldLocationToScreen(WorldLocation, Location);
		Location.X = FMath::RandRange(Location.X - 100.0f, Location.X + 100.0f);
		Location.Y = FMath::RandRange(Location.Y - 100.0f, Location.Y + 100.0f);
	}

	//@Setting - Color
	{
		(UIColor == EFloatingComboColor::RED)
			? Color = FLinearColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f))
			: Color = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

FSlateColor UWG_FloatingCombo::GetInitialColor() const
{
	FVector Test = FVector(Color.R, Color.G, Color.B);
	CLog::Print(Test);
	return FSlateColor(FLinearColor(Color));
}
