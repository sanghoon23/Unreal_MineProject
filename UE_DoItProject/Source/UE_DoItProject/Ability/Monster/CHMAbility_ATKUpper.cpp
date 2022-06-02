#include "CHMAbility_ATKUpper.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_MeshParticle.h"
#include "Charactor/Monster/Base/CHumanoidMonster.h"

UCHMAbility_ATKUpper::UCHMAbility_ATKUpper()
{
	AbilityType = EAbilityType::ATKUP;

	FString strPath = L"";

	//LOAD UI Texture 
	{
		strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_ATKUpFill.Tex_ATKUpFill'";
		ConstructorHelpers::FObjectFinder<UTexture2D> ATKUpTexture(*strPath);
		if (ATKUpTexture.Succeeded())
		{
			TextureUI = ATKUpTexture.Object;
		}
	}

	//@Set UI Color
	{
		//@긍정적 효과
		ColorAndOpacity = FLinearColor(FVector4(0.0f, 1.0f, 0.0f, 1.5f));
		TintColor = FLinearColor(FVector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UCHMAbility_ATKUpper::StartUseTimerAbility()
{
	Super::StartUseTimerAbility();
	check(AppliedActor);

	IIC_Monster* I_Monster = Cast<IIC_Monster>(AppliedActor);
	if (I_Monster != nullptr)
	{
		//@공격 계수 증가 '+'
		{
			ACHumanoidMonster* HumanoidMonster = Cast<ACHumanoidMonster>(AppliedActor);
			check(HumanoidMonster);

			//@더하기
			HumanoidMonster->AddATK(ATKUpperCoefficient);
		}
	}
	else UE_LOG(LogTemp, Warning, L"UCHMAbility_ATKUpper I_Monster NULL!!");

}

void UCHMAbility_ATKUpper::EndUseTimerAbility()
{
	Super::EndUseTimerAbility();
	check(AppliedActor);

	IIC_Monster* I_Monster = Cast<IIC_Monster>(AppliedActor);
	if (I_Monster != nullptr)
	{
		//@공격 계수 증가 '+'
		{
			ACHumanoidMonster* HumanoidMonster = Cast<ACHumanoidMonster>(AppliedActor);
			check(HumanoidMonster);

			ATKUpperCoefficient *= (-1); //@빼기
			HumanoidMonster->AddATK(ATKUpperCoefficient);
		}
	}
	else UE_LOG(LogTemp, Warning, L"UCHMAbility_ATKUpper I_Monster NULL!!");
}

void UCHMAbility_ATKUpper::OverlapAbility(UCBaseAbility * Ability)
{
	Super::OverlapAbility(Ability);
	check(Ability);

	if (AbilityType != Ability->GetAbilityType())
	{
		UE_LOG(LogTemp, Warning, L"UCPLAbility_SpeedUpper OverlapAbility Function Same NOT TYPE!!");
		return;
	}

	EndUseTimerAbility();

	StartUseTimerAbility();
}

void UCHMAbility_ATKUpper::ApplyAbility()
{
	check(AppliedActor);

	IIC_Monster* I_Monster = Cast<IIC_Monster>(AppliedActor);
	if (I_Monster != nullptr)
	{
		//@공격 계수 증가 '+'
		{
			ACHumanoidMonster* HumanoidMonster = Cast<ACHumanoidMonster>(AppliedActor);
			check(HumanoidMonster);

			//@더하기
			HumanoidMonster->AddATK(ATKUpperCoefficient);
		}
	}
	else UE_LOG(LogTemp, Warning, L"UCHMAbility_ATKUpper I_Monster NULL!!");
}
