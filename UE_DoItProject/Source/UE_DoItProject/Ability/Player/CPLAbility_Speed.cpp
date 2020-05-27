#include "CPLAbility_Speed.h"
#include "Global.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Player.h"
#include "Interface/IC_MeshParticle.h"
#include "Charactor/Player/CPlayer.h"

UCPLAbility_Speed::UCPLAbility_Speed()
{
	AbilityType = EAbilityType::SPEED;

	FString strPath = L"";
	//@LOAD Particle
	{
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

void UCPLAbility_Speed::StartUseTimerAbility()
{
	Super::StartUseTimerAbility();

	check(AppliedActor);
	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@기존 Player Particle OFF
		I_Player->OffParticleInPlayer();

		//@ '+' Speed
		{
			ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
			check(Player);
			float InputValue = AbilityValue.Value;
			Player->AddSpeedToOrigin(InputValue); //@Add Speed
		}
	}
	else
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeed I_Player NULL!!");

	//@파티클 적용
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(AppliedActor);
	if (I_Charactor != nullptr)
	{
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			//@LHand ParticleComp
			ParticleComp_LHand = I_MeshParticle->SpawnParticleAtMesh(SpeedUp_LHand, AttachPointType::LHAND, EAttachLocation::SnapToTarget);

			//@RHand ParticleComp
			ParticleComp_RHand = I_MeshParticle->SpawnParticleAtMesh(SpeedUp_RHand, AttachPointType::RHAND, EAttachLocation::SnapToTarget);
		}
	}
	
}

void UCPLAbility_Speed::EndUseTimerAbility()
{
	Super::EndUseTimerAbility();

	check(AppliedActor);
	IIC_Player* I_Player = Cast<IIC_Player>(AppliedActor);
	if (I_Player != nullptr)
	{
		//@기존 Player Particle ON
		I_Player->OnParticleInPlayer();

		//@ '-' Speed
		{
			ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
			check(Player);
			float InputValue = (-1) * AbilityValue.Value;
			Player->AddSpeedToOrigin(InputValue);
		}
	}
	else
		UE_LOG(LogTemp, Warning, L"UCPLAblilitySpeed I_Player NULL!!");

	//@Particle OFF
	if (ParticleComp_LHand != nullptr)
		ParticleComp_LHand->SetActive(false);

	if (ParticleComp_RHand != nullptr)
		ParticleComp_RHand->SetActive(false);

	//@Garbege Collection
	//BeginDestroy();
}

bool UCPLAbility_Speed::OverlapAbility(UCBaseAbility * Ability)
{
	Super::OverlapAbility(Ability);
	check(Ability);

	if (AbilityType != Ability->GetAbilityType())
	{
		UE_LOG(LogTemp, Warning, L"UCPLAbility_Speed OverlapAbility Function Same NOT TYPE!!");
		return false;
	}

	//@Warning - Value += 해주면 안됨.
	const FAbilityValue& InputAbilityValue = Ability->GetAbilityValue();
	AbilityValue.Timer += InputAbilityValue.Timer;

	return true;
}

//void UCPLAbility_Speed::ApplyAbility()
//{
//	check(AppliedActor);
//}
