#include "CUpset_Stun.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "Interface//IC_MeshParticle.h"

UCUpset_Stun::UCUpset_Stun()
{
	//Super
	State = EHitUpset::STUN;
}

void UCUpset_Stun::StartCondition(APawn * Owner)
{
	Super::StartCondition(Owner);
	check(Owner);

	//@Stun Head Particle �Ӹ��� ���̱�
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Owner);
	if (I_Charactor != nullptr)
	{
		IIC_MeshParticle* I_MeshParticle = I_Charactor->GetIMeshParticle();
		if (I_MeshParticle != nullptr)
		{
			StunHeadParticleComp = I_MeshParticle->SpawnParticleAtMesh
			(
				StunHeadParticle, 
				EAttachPointType::HEAD,
				EAttachPointRelative::NONE,
				EAttachLocation::SnapToTargetIncludingScale
			);

			if (StunHeadParticleComp == nullptr)
				UE_LOG(LogTemp, Warning, L"CUpset_Stun StunHeadParticleComp NULL!!");
		}
	}

}

void UCUpset_Stun::UpdateCondition(APawn * Owner, float DeltaTime)
{
	//@ApplyTime -= DeltaTime
	Super::UpdateCondition(Owner, DeltaTime);

	check(Owner);
	//@TakeDamage
	// ...

	ACharacter* Charactor = Cast<ACharacter>(Owner);
	if (Charactor != nullptr)
	{
		UAnimInstance* AnimInst = Charactor->GetMesh()->GetAnimInstance();
		check(AnimInst);

		//@�ٸ� ��Ÿ�ְ� ����ǰ� �ִ���
		bool IsOtherMonPlaying = AnimInst->IsAnyMontagePlaying();

		//@NonAction �� ������ Montage �� ����Ǵ���,
		bool IsPlayingNonAction = AnimInst->Montage_IsPlaying(NonActionMon);

		//@JumpSection - ���� ApplyTime �� �����ִٸ�,
		IIC_Monster* I_Monster = Cast<IIC_Monster>(Charactor);
		if (ApplyTime > 0.0f && IsOtherMonPlaying == false)
		{
			//@AI OFF
			I_Monster->SetAIRunningPossible(false);

			//@RUN Montage
			IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
			if (I_Charactor != nullptr)
			{
				I_Charactor->ActorAnimMonPlay(NonActionMon, 0.6f, true);
				AnimInst->Montage_JumpToSection
				(
					FName("Looping"), NonActionMon //@Looping
				);
			}
		}
		//@ApplyTime �� �����ٸ�,
		else if (ApplyTime <= 0.0f)
		{
			if (IsPlayingNonAction == true)
			{
				AnimInst->Montage_Stop(0.5f, NonActionMon);
			}

			//@AI ON
			I_Monster->SetAIRunningPossible(true);
		}
	}
}

void UCUpset_Stun::EndCondition(APawn * Owner)
{
	Super::EndCondition(Owner);
	check(Owner);

	//@Particle OFF
	if (StunHeadParticleComp != nullptr)
	{
		StunHeadParticleComp->SetActive(false);
		StunHeadParticleComp = nullptr;
	}
	else
		UE_LOG(LogTemp, Warning, L"CUpset_Stun EndCondition StunHeadParticleComp NULL!!");
}

void UCUpset_Stun::ConditionOverlap(UCBaseConditionType * OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	//@�ʱ�ȭ
	ApplyTime = OverlappedCondition->ApplyTime;
	InitUIColorAndOpacity();
}

void UCUpset_Stun::SetStunHeadPrticle(UParticleSystem * PT)
{
	check(PT);
	StunHeadParticle = PT;
}

void UCUpset_Stun::SetMontage(UAnimMontage * Montage)
{
	check(Montage);
	NonActionMon = Montage;
}
