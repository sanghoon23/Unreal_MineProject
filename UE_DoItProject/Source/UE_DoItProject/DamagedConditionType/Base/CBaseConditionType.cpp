#include "CBaseConditionType.h"
#include "Global.h"

void UCBaseConditionType::UpdateCondition(APawn * Owner, float DeltaTime)
{
	check(Owner);

	//@Apply Time
	ApplyTime -= DeltaTime;

	if (ApplyTime < OpacityLinearTimer)
	{
		UpdateUIColorAndOpacity(this);
	}
}

void UCBaseConditionType::EndCondition(APawn * Owner)
{
	//@Init
	ColorAndOpacity = FLinearColor(FVector4(1.0f));
	bLinerColorDir = true;
	TextureUI = nullptr;
	ApplyTime = 0.0f;
}

void UCBaseConditionType::SetDamageSubjectController(AController * InputController)
{
	check(InputController);
	DamageSubjectController = InputController;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@ UI

void UCBaseConditionType::InitUIColorAndOpacity()
{
	ColorAndOpacity = FLinearColor(FVector4(1.0f));
	bLinerColorDir = true;
}

void UCBaseConditionType::UpdateUIColorAndOpacity(UCBaseConditionType* ConditionData)
{
	float Opacity = ConditionData->ColorAndOpacity.A;

	(ConditionData->bLinerColorDir == true)
		? Opacity += OpacityLinearSpeed
		: Opacity -= OpacityLinearSpeed;

	if (Opacity > 1.0f || Opacity < 0.1f)
	{
		ConditionData->bLinerColorDir = !(ConditionData->bLinerColorDir);
	}

	ConditionData->ColorAndOpacity.A = Opacity;
}

void UCBaseConditionType::SetTextureUI(UTexture2D * Texture)
{
	check(Texture);
	TextureUI = Texture;
}
