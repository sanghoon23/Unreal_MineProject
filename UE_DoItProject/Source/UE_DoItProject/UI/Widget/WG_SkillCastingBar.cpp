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

	//@Forward (Left To Right)
	if (bPlaying == true)
	{
		//@Calc
		(bReverse == false)
			? Persentage += FillSpeed * InDeltaTime
			: Persentage -= FillSpeed * InDeltaTime;

		//Test Code
		//SetRenderOpacity(0.0f);

		//@float 인접한 FMath 알아보기, Persentage - ( 0 .. 1 )
		if (Persentage > 1.0f || Persentage <= 0.001f)
		{
			bPlaying = false;
			Persentage = 0.0f;

			bFinishFilledProgress = true;

			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UWG_SkillCastingBar::StartProgress(float FillingSpeed, float StartPercentage, bool Reverse)
{
	bPlaying = true;
	bFinishFilledProgress = false;

	//@Set Value
	Persentage = StartPercentage;
	FillSpeed = FillingSpeed;
	bReverse = Reverse;

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UWG_SkillCastingBar::EndProgress()
{
	bPlaying = false;

	//@Set Value
	Persentage = 0.0f;
	FillSpeed = 1.0f;
	bReverse = false;

	SetVisibility(ESlateVisibility::Hidden);
}
