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

	//@LOAD Particle - SlowerDamage
	{
		//@Root
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Root.PS_Slower_FromFreeze_Root'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_1(*strPath);
		if (SlowerPT_1.Succeeded())
			SlowerParticle_Root = SlowerPT_1.Object;

		//@Body
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Damaged/PS_Slower_FromFreeze_Body.PS_Slower_FromFreeze_Body'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> SlowerPT_2(*strPath);
		if (SlowerPT_2.Succeeded())
			SlowerParticle_Body = SlowerPT_2.Object;
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


	//@Spawn Particle
	IIC_Charactor* IC_Charactor = Cast<IIC_Charactor>(AppliedActor);
	check(IC_Charactor);

	IIC_MeshParticle* I_MeshParticle = IC_Charactor->GetIMeshParticle();
	check(I_MeshParticle);

	FTransform RootTrans = FTransform::Identity;
	RootTrans.SetScale3D(FVector(2.0f));
	PTComp_SlowerRoot = I_MeshParticle->SpawnParticleAtMesh
	(
		SlowerParticle_Root,
		EAttachPointType::ROOT,
		EAttachPointRelative::NONE,
		EAttachLocation::SnapToTarget,
		RootTrans
	);

	FTransform BodyTrans = FTransform::Identity;
	BodyTrans.SetScale3D(FVector(2.0f));
	PTComp_SlowerBody = I_MeshParticle->SpawnParticleAtMesh
	(
		SlowerParticle_Body,
		EAttachPointType::ROOT,
		EAttachPointRelative::NONE,
		EAttachLocation::SnapToTarget,
		BodyTrans
	);

}

void UCPLAbility_SpeedDown::EndUseTimerAbility()
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
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeedDown I_Player NULL!!");

	//@Disable ParticleComponent
	PTComp_SlowerRoot->SetActive(false);
	PTComp_SlowerBody->SetActive(false);
}

void UCPLAbility_SpeedDown::OverlapAbility(UCBaseAbility * Ability)
{
	Super::OverlapAbility(Ability);
	check(Ability);

	if (AbilityType != Ability->GetAbilityType())
	{
		UE_LOG(LogTemp, Warning, L"UCPLAbility_SpeedDowner OverlapAbility Function Same NOT TYPE!!");
		return;
	}

	EndUseTimerAbility();

	StartUseTimerAbility();
}
