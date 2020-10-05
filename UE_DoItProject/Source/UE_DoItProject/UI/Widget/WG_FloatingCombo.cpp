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

	//TODO : 다른 게임 UI 나오는 부분 참고해보자. 뭔가 엉성하다.
	//PlayerTarget 일 때, 데미지계수 나오는 것은 확인됬다.
	//Test Code
	//if (Owner != nullptr)
	//{
	//	FVector OwnerWorldLocation = Owner->GetActorLocation();
	//	OwnerWorldLocation.Z += Owner->GetDefaultHalfHeight() + 100.0f;

	//	PlayerController->ProjectWorldLocationToScreen(OwnerWorldLocation, Location);
	//}
}

void UWG_FloatingCombo::SetInitial(APlayerController* PC, FVector WorldLocation, EFloatingComboColor UIColor, bool bRandomPos)
{
	check(PC);
	PlayerController = PC;

	//@Setting - Random Range
	{
		PC->ProjectWorldLocationToScreen(WorldLocation, Location);

		if (bRandomPos == true)
		{
			Location.X = FMath::RandRange(Location.X - 100.0f, Location.X + 100.0f);
			Location.Y = FMath::RandRange(Location.Y - 100.0f, Location.Y + 100.0f);
		}		
	}

	//SetRenderTranslation(Location);

	//@Setting - Color
	{
		(UIColor == EFloatingComboColor::RED)
			? Color = FLinearColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f))
			: Color = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

FLinearColor UWG_FloatingCombo::GetInitialColor() const
{
	FVector Test = FVector(Color.R, Color.G, Color.B);
	return FLinearColor(Color);
}
