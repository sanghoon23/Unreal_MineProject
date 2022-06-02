#include "CUpset_Freeze.h"
#include "Global.h"
#include "GameFramework/Character.h"
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

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		//@Montage DontPlay
		I_Charactor->SetDontMontagePlay(true);

		//@Montage Pause
		I_Charactor->ActorAnimMonPause();

		//@Animation Pause
		ACharacter* CH = Cast<ACharacter>(Owner);
		check(CH);
		//CH->GetMesh()->GetAnimInstance()->StopSlotAnimation(0.25f, "DefaultSlot");
		CH->GetMesh()->GetAnimInstance()->GetOwningComponent()->GlobalAnimRateScale = 0.0f;

		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			//@Upper Particle
			check(FreezeParticle);
			FreezeParticleComp = I_MeshParticle->SpawnParticleAtMesh
			(
				FreezeParticle, 
				EAttachPointType::BODY,
				EAttachPointRelative::NONE,
				EAttachLocation::SnapToTargetIncludingScale
			);

			if (FreezeParticleComp == nullptr)
				UE_LOG(LogTemp, Warning, L"CUpset_Freeze FreezeParticleComp NULL!!");

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			//@Under Particle
			check(FreezeUnderParticle);
			FreezeUnderParticleComp = I_MeshParticle->SpawnParticleAtMesh
			(
				FreezeUnderParticle,
				EAttachPointType::ROOT,
				EAttachPointRelative::NONE,
				EAttachLocation::SnapToTargetIncludingScale
			);

			if (FreezeUnderParticleComp == nullptr)
				UE_LOG(LogTemp, Warning, L"CUpset_Freeze FreezeUnderParticleComp NULL!!");

		}
	}//(I_Charactor != nullptr)
}

void UCUpset_Freeze::UpdateCondition(APawn * Owner, float DeltaTime)
{
	Super::UpdateCondition(Owner, DeltaTime);
	check(Owner);

	//@Particle Freeze Update
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		//@만약 얼고 있는 도중에 죽었다면,
		//ApplyTime < 0.0f 만들고, HitComp Tick 에서 EndCondition 을 호출.
		if (I_Charactor->IsDeath() == true)
		{
			ApplyTime -= DeltaTime;
		}
	}
}

void UCUpset_Freeze::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	//@Particle OFF
	{
		if (FreezeParticleComp != nullptr)
		{
			FreezeParticleComp->SetActive(false);
			FreezeParticleComp = nullptr;
		}
		else
			UE_LOG(LogTemp, Warning, L"CUPset_Freeze EndCondition FreezeParticleComp NULL!!");

		if (FreezeUnderParticleComp != nullptr)
		{
			FreezeUnderParticleComp->SetActive(false);
			FreezeUnderParticleComp = nullptr;
		}
		else
			UE_LOG(LogTemp, Warning, L"CUPset_Freeze EndCondition FreezeUnderParticleComp NULL!!");
	}

	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		//@Animation Resume
		ACharacter* CH = Cast<ACharacter>(Owner);
		check(CH);
		CH->GetMesh()->GetAnimInstance()->GetOwningComponent()->GlobalAnimRateScale = 1.0f;

		//@Montage DontPlay
		I_Charactor->SetDontMontagePlay(false);

		//@Montage Resume
		I_Charactor->ActorPausedAnimMonResume();
	}

	//@Spawn Destructible Mesh
	{
		UWorld* const World = Owner->GetWorld();
		FTransform Transform = FTransform::Identity;
		FActorSpawnParameters Params;
		Params.Owner = Owner;

		ACDM_FreezingBroken* DM_FreezenBroken = World->SpawnActor<ACDM_FreezingBroken>(ACDM_FreezingBroken::StaticClass(), Transform, Params);
		check(DM_FreezenBroken);

		DM_FreezenBroken->SetActorEnableCollision(false);
		DM_FreezenBroken->SetActorLocation(Owner->GetActorLocation());

		UDestructibleComponent* Destructible = DM_FreezenBroken->GetDestructibleComponent();
		if (Destructible != nullptr)
		{
			Destructible->ApplyDamage(50.0f, DM_FreezenBroken->GetActorLocation(), Owner->GetActorForwardVector(), 1.0f);
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
}

void UCUpset_Freeze::SetFreezeParticle(UParticleSystem* PT)
{
	check(PT);
	if (PT != nullptr)
	{
		FreezeParticle = PT;
	}
}

void UCUpset_Freeze::SetFreezeUnderParticle(UParticleSystem * PT)
{
	check(PT);
	if (PT != nullptr)
	{
		FreezeUnderParticle = PT;
	}
}
