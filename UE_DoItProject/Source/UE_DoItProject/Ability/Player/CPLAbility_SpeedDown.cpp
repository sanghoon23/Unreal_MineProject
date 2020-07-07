#include "CPLAbility_SpeedDown.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_MeshParticle.h"
#include "Charactor/Player/CPlayer.h"

UCPLAbility_SpeedDown::UCPLAbility_SpeedDown()
{
	AbilityType = EAbilityType::SPEEDDOWN;

	FString strPath = L"";

	//LOAD UI Texture 
	{
		strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_SpeedTypeFill.Tex_SpeedTypeFill'";
		ConstructorHelpers::FObjectFinder<UTexture2D> SpeedDownTexture(*strPath);
		if (SpeedDownTexture.Succeeded())
		{
			TextureUI = SpeedDownTexture.Object;
		}
	}

	//@Set UI Color
	{
		//@부정적 효과
		ColorAndOpacity = FLinearColor(FVector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

void UCPLAbility_SpeedDown::StartUseTimerAbility()
{
	Super::StartUseTimerAbility();
	check(AppliedActor);

	//@Onwer 가 다를 수 있음
	OnDelStartTimerAbility.Broadcast(AppliedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@ '+' Speed
		{
			ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
			check(Player);
			float InputValue = AbilityValue.Value;
			Player->AddSpeedToOrigin(InputValue); //@Add Speed
		}
	}
	else
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeedDonwer I_Player NULL!!");

}

void UCPLAbility_SpeedDown::EndUseTimerAbility()
{
	Super::EndUseTimerAbility();
	check(AppliedActor);

	//@Onwer 가 다를 수 있음
	OnEndTimerAbility.Broadcast(AppliedActor);

	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@ '-' Speed
		{
			ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
			check(Player);
			float InputValue = (-1) * AbilityValue.Value;
			Player->AddSpeedToOrigin(InputValue);
		}
	}
	else
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeedDown I_Player NULL!!");
}

void UCPLAbility_SpeedDown::OverlapAbility(UCBaseAbility * Ability)
{
	Super::OverlapAbility(Ability);
	check(Ability);

	if (AbilityType != Ability->GetAbilityType())
	{
		UE_LOG(LogTemp, Warning, L"UCPLAbility_SpeedDowner OverlapAbility Function Same NOT TYPE!!");
	}

	EndUseTimerAbility();

	//@Copy
	{
		Copy(Ability); //@Copy - DelegateRemove 수행
	}

	Ability->StartUseTimerAbility();
}
