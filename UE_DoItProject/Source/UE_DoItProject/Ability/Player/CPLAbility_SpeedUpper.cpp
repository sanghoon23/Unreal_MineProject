#include "CPLAbility_SpeedUpper.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_MeshParticle.h"
#include "Charactor/Player/CPlayer.h"

UCPLAbility_SpeedUpper::UCPLAbility_SpeedUpper()
{
	AbilityType = EAbilityType::SPEEDUP;

	FString strPath = L"";

	//LOAD UI Texture 
	{
		strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_SpeedTypeFill.Tex_SpeedTypeFill'";
		ConstructorHelpers::FObjectFinder<UTexture2D> SpeedUpTexture(*strPath);
		if (SpeedUpTexture.Succeeded())
		{
			TextureUI = SpeedUpTexture.Object;
		}
	}

	//@Set UI Color
	{
		//@긍정적 효과
		ColorAndOpacity = FLinearColor(FVector4(0.0f, 1.0f, 1.0f, 1.0f));
	}

	//@LOAD Particle
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_FasterEffect.PS_FasterEffect'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Faster(*strPath);
		if (PS_Faster.Succeeded())
		{
			FasterParticle = PS_Faster.Object;
		}

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_ApplyFasterWithHand.PS_ApplyFasterWithHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_LHandSpeedUp(*strPath);
		if (P_LHandSpeedUp.Succeeded())
		{
			SpeedUp_LHand = P_LHandSpeedUp.Object;
		}

		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_ApplyFasterWithHand.PS_ApplyFasterWithHand'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_RHandSpeedUp(*strPath);
		if (P_RHandSpeedUp.Succeeded())
		{
			SpeedUp_RHand = P_RHandSpeedUp.Object;
		}
	}
}

void UCPLAbility_SpeedUpper::StartUseTimerAbility()
{
	Super::StartUseTimerAbility();
	check(AppliedActor);

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
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeedUpper I_Player NULL!!");


	//@파티클 실행
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(AppliedActor);
	check(I_Charactor);

	IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
	check(I_MeshParticle);

	I_MeshParticle->SpawnParticleAtMesh
	(
		FasterParticle,
		EAttachPointType::ROOT,
		EAttachPointRelative::NONE,
		EAttachLocation::SnapToTarget
	);

	//@LHand ParticleComp
	PTComp_LHand = I_MeshParticle->SpawnParticleAtMesh
	(
		SpeedUp_LHand,
		EAttachPointType::LHAND,
		EAttachPointRelative::RELATIVE,
		EAttachLocation::SnapToTarget
	);

	//@RHand ParticleComp
	PTComp_RHand = I_MeshParticle->SpawnParticleAtMesh
	(
		SpeedUp_RHand,
		EAttachPointType::RHAND,
		EAttachPointRelative::RELATIVE,
		EAttachLocation::SnapToTarget
	);
}

void UCPLAbility_SpeedUpper::EndUseTimerAbility()
{
	Super::EndUseTimerAbility();
	check(AppliedActor);

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
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeedUpper I_Player NULL!!");

	PTComp_LHand->SetActive(false);
	PTComp_RHand->SetActive(false);
}

//#Edit 0703 - 
/* 기존의 Ability 는 지운다.. */
void UCPLAbility_SpeedUpper::OverlapAbility(UCBaseAbility * Ability)
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
