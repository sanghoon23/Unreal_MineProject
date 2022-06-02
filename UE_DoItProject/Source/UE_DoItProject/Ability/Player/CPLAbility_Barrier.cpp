#include "CPLAbility_Barrier.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_MeshParticle.h"
#include "Charactor/Player/CPlayer.h"

UCPLAbility_Barrier::UCPLAbility_Barrier()
{
	AbilityType = EAbilityType::BARRIER;

	FString strPath = L"";

	//LOAD UI Texture 
	{
		strPath = L"Texture2D'/Game/_Mine/_MyBlueprint/Texture/UI/ConditionTexture_Filling/Tex_AmorTypeFill.Tex_AmorTypeFill'";
		ConstructorHelpers::FObjectFinder<UTexture2D> BarrierTexture(*strPath);
		if (BarrierTexture.Succeeded())
		{
			TextureUI = BarrierTexture.Object;
		}
	}

	//@Set UI Color
	{
		//@±àÁ¤Àû È¿°ú
		ColorAndOpacity = FLinearColor(FVector4(0.0f, 1.0f, 1.0f, 1.0f));
	}

	//@LOAD Particle
	{
		strPath = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/Action/PS_BarrierEffect.PS_BarrierEffect'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_Barrier(*strPath);
		if (P_Barrier.Succeeded())
		{
			ParticleBarrier = P_Barrier.Object;
		}
	}
}

void UCPLAbility_Barrier::StartUseTimerAbility()
{
	Super::StartUseTimerAbility();

	check(AppliedActor);
	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@ '+' Barrier
		{
			ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
			check(Player);
			float InputValue = AbilityValue.Value;
			Player->AddBarrierAmount(InputValue); //@Add Barrier Amount
		}
	}
	else
		UE_LOG(LogTemp, Warning, L"UCPLAbility_Barrier I_Player NULL!!");

	//@Create Particle
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(AppliedActor);
	check(I_Charactor);

	IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
	check(I_MeshParticle);

	PTComp_Barrier = I_MeshParticle->SpawnParticleAtMesh
	(
		ParticleBarrier,
		EAttachPointType::BODY,
		EAttachPointRelative::NONE,
		EAttachLocation::SnapToTarget
	);
}

void UCPLAbility_Barrier::TickUseTimerAbility(float DeltaTime)
{
	Super::TickUseTimerAbility(DeltaTime);

	ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
	if (Player != nullptr)
	{
		const FPlayerInfo& PL_Info = Player->GetPlayerInfo();
		
		float BarrierAmount = PL_Info.BarrierAmount;
		if (BarrierAmount <= 0.0f)
		{
			AbilityValue.Timer = 0.0f;
		}
	}
}

void UCPLAbility_Barrier::EndUseTimerAbility()
{
	Super::EndUseTimerAbility();

	check(AppliedActor);

	//@Barrier OFF
	ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
	if (Player != nullptr)
	{
		const FPlayerInfo& PL_Info = Player->GetPlayerInfo();

		float BarrierAmount = PL_Info.BarrierAmount;
		if (BarrierAmount > 0.0f)
		{
			BarrierAmount *= (-1);
			Player->AddBarrierAmount(BarrierAmount);
		}
	}

	PTComp_Barrier->SetActive(false);
}

void UCPLAbility_Barrier::OverlapAbility(UCBaseAbility * Ability)
{
	Super::OverlapAbility(Ability);
	check(Ability);

	if (AbilityType != Ability->GetAbilityType())
	{
		UE_LOG(LogTemp, Warning, L"UCPLAbility_Barrier OverlapAbility Function Same NOT TYPE!!");
		return;
	}

	//@ '+' Barrier
	{
		ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
		check(Player);
		float InputValue = Ability->GetAbilityValue().Value;
		Player->AddBarrierAmount(InputValue);
	}

	float BarrierAmount = Ability->GetAbilityValue().Value;
	float BarrierTimer = Ability->GetAbilityValue().Timer;

	AbilityValue.Value += BarrierAmount;

	if (BarrierTimer > AbilityValue.Timer)
	{
		AbilityValue.Timer = BarrierTimer;
	}
}
