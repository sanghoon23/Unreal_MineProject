#include "CBaseConditionType.h"
#include "Global.h"

UCBaseConditionType::UCBaseConditionType()
{
}

void UCBaseConditionType::Copy(const UCBaseConditionType * const In)
{
	ColorAndOpacity = In->ColorAndOpacity;
	TintColor = In->TintColor;
	ApplyTime = In->ApplyTime;
	bLinerColorDir = In->bLinerColorDir;

	SetState(In->GetState());
	SetDamageEvent(In->GetDamageEvent());
	SetDamageSubjectController(In->DamageSubjectController);

	OpacityLinearTimer = In->OpacityLinearTimer;
	OpacityLinearSpeed = In->OpacityLinearSpeed;
}

void UCBaseConditionType::StartCondition(APawn * Owner)
{
	check(Owner);
	OnDelStartCondition.Broadcast(Owner);
}

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
	check(Owner);
	OnDelEndCondition.Broadcast(Owner);

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
