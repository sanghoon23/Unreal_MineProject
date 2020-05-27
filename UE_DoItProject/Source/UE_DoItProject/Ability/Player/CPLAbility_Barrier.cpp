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

	//@파티클 적용
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(AppliedActor);
	if (I_Charactor != nullptr)
	{
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			//@Barrier ParticleComp
			ParticleComp_Barrier = I_MeshParticle->SpawnParticleAtMesh(ParticleBarrier, AttachPointType::BODY, EAttachLocation::SnapToTarget);
		}
	}
}

void UCPLAbility_Barrier::TickUseTimerAbility()
{
	Super::TickUseTimerAbility();

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

	//@Particle OFF
	if (ParticleComp_Barrier != nullptr)
		ParticleComp_Barrier->SetActive(false);
}

bool UCPLAbility_Barrier::OverlapAbility(UCBaseAbility * Ability)
{
	Super::OverlapAbility(Ability);
	check(Ability);

	if (AbilityType != Ability->GetAbilityType())
	{
		UE_LOG(LogTemp, Warning, L"UCPLAbility_Speed OverlapAbility Function Same NOT TYPE!!");
		return false;
	}

	//@ '+' Barrier
	{
		ACPlayer* Player = Cast<ACPlayer>(AppliedActor);
		check(Player);
		float InputValue = Ability->GetAbilityValue().Value;
		Player->AddBarrierAmount(InputValue); //@Add Barrier Amount
	}

	const FAbilityValue& InputAbilityValue = Ability->GetAbilityValue();
	AbilityValue.Timer += InputAbilityValue.Timer;

	return true;
}

