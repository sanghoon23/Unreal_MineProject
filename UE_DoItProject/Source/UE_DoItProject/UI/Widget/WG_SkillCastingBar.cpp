#include "WG_SkillCastingBar.h"
#include "CLog.h"
#include "UMG.h"

UWG_SkillCastingBar::UWG_SkillCastingBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UWG_SkillCastingBar::NativeConstruct()
{
	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();

}

void UWG_SkillCastingBar::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlaying == true)
	{
		//@Calc
		Persentage += FillSpeed * InDeltaTime;

		//Test Code
		//SetRenderOpacity(0.0f);

		//@float 인접한 FMath 알아보기, Persentage - ( 0 .. 1 )
		if (Persentage > 1.0f)
		{
			bPlaying = false;
			Persentage = 0.0f;

			bFinishFilledProgress = true;

			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UWG_SkillCastingBar::StartProgress()
{
	bPlaying = true;
	bFinishFilledProgress = false;

	//@Set Value
	Persentage = 0.0f;

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWG_SkillCastingBar::EndProgress()
{
	bPlaying = false;

	//@Set Value
	Persentage = 0.0f;

	SetVisibility(ESlateVisibility::Hidden);
}
