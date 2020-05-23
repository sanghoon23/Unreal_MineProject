#include "CUpset_Freeze.h"
#include "Global.h"
#include "DestructibleComponent.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_HitComp.h"
#include "Interface/IC_Monster.h"
#include "Interface/IC_MeshParticle.h"

#include "Environment/Destructible/CDM_FreezingBroken.h"

UCUpset_Freeze::UCUpset_Freeze()
{
	//Super
	State = EHitUpset::FREEZE;
}

void UCUpset_Freeze::StartCondition(APawn * Owner)
{
	Super::StartCondition(Owner);
	check(Owner);

	IfTrueRet(FreezeParticleComp == nullptr);

	CLog::Print(L"Freeze StartCondition!!");

	bool bSet = false;
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		//@Montage Pause
		I_Charactor->ActorAnimMonPause();

		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			bSet = I_MeshParticle->SetLocationParticleCompAtMesh(FreezeParticleComp, AttachPointType::BODY);
		}

		//@빙결 상황일 땐 다른 Montage 가 들어오지 못하도록 함.
		IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
		if (I_HitComp != nullptr)
		{
			I_HitComp->SetBlockDamagedMontage(true); //@TRUE
		}

	}//(I_Charactor != nullptr)

	if (bSet == true)
	{
		FreezeParticleComp->SetActive(true);
	}
	else
		UE_LOG(LogTemp, Warning, L"UCUpset_Freeze StartCondition NOT SET ParticleLocation");

	///
	IIC_Monster* I_Monster = Cast<IIC_Monster>(Owner);
	if (I_Monster != nullptr)
	{
		//@AI OFF
		I_Monster->SetAIRunningPossible(false);
	}
}

void UCUpset_Freeze::UpdateCondition(APawn * Owner, float DeltaTime)
{
	Super::UpdateCondition(Owner, DeltaTime);

	IfTrueRet(FreezeParticleComp == nullptr);

	//@Particle Freeze Update
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		//@만약 얼고 있는 도중에 죽었다면,
		//ApplyTime < 0.0f 만들고, HitComp Tick 에서 EndCondition 을 호출.
		if (I_Charactor->IsDeath() == true)
		{
			ApplyTime = -0.001f;
		}

		//@Freeze Particle
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			I_MeshParticle->SetLocationParticleCompAtMesh(FreezeParticleComp, AttachPointType::BODY);
		}
	}
}

void UCUpset_Freeze::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	IfTrueRet(FreezeParticleComp == nullptr);

	//@Particle OFF
	FreezeParticleComp->SetActive(false);
	FreezeParticleComp = nullptr;

	IIC_Monster* I_Monster = Cast<IIC_Monster>(Owner);
	if (I_Monster != nullptr)
	{
		//@AI ON
		I_Monster->SetAIRunningPossible(true);
	}

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		//@Montage Resume
		I_Charactor->ActorPausedAnimMonResume();

		//@빙결이 끝났을 땐 Block 해제
		IIC_HitComp* I_HitComp = I_Charactor->GetIHitComp();
		if (I_HitComp != nullptr)
		{
			I_HitComp->SetBlockDamagedMontage(false); //@FALSE
		}
	}

	//@Spawn Destructible Mesh
	{
		UWorld* const World = Owner->GetWorld();
		FTransform Transform = FTransform::Identity;
		FActorSpawnParameters Params;
		Params.Owner = Owner;

		DM_FreezenBroken = World->SpawnActor<ACDM_FreezingBroken>(ACDM_FreezingBroken::StaticClass(), Transform, Params);
		check(DM_FreezenBroken);

		DM_FreezenBroken->SetActorLocation(Owner->GetActorLocation());

		UDestructibleComponent* Destructible
			= DM_FreezenBroken->GetDestructibleComponent();
		if (Destructible != nullptr)
		{
			//FVector HitLocation
			Destructible->ApplyDamage(100.0f, DM_FreezenBroken->GetActorLocation(), Owner->GetActorForwardVector(), 1.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, L"CUpset_Freeze Destructible Mesh NULL!!");
		}
	}
}

void UCUpset_Freeze::ConditionOverlap(UCBaseConditionType* OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	UCUpset_Freeze* OverlapCondition = Cast<UCUpset_Freeze>(OverlappedCondition);
	check(OverlapCondition);

	//@초기화
	ApplyTime = OverlapCondition->ApplyTime;
	InitUIColorAndOpacity();

	CLog::Print(L"Freeze Overlap!!");
}

void UCUpset_Freeze::SetFreezeParticleComp(UParticleSystemComponent * PTComp)
{
	check(PTComp);
	if (PTComp != nullptr)
	{
		FreezeParticleComp = PTComp;
	}
}
