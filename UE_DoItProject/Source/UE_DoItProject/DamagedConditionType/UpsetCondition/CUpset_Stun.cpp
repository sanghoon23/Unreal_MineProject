#include "CUpset_Stun.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"

UCUpset_Stun::UCUpset_Stun()
{
	//Super
	State = EHitUpset::STUN;
}

void UCUpset_Stun::UpdateConditionOnActor(AActor * Owner, float DeltaTime)
{
	//@ApplyTime -= DeltaTime
	Super::UpdateConditionOnActor(Owner, DeltaTime);

	check(Owner);
	//@TakeDamage
	// ...

	ACharacter* Charactor = Cast<ACharacter>(Owner);
	if (Charactor != nullptr)
	{
		UAnimInstance* AnimInst = Charactor->GetMesh()->GetAnimInstance();
		check(AnimInst);

		//@다른 몽타주가 실행되고 있는지
		bool IsOtherMonPlaying = AnimInst->IsAnyMontagePlaying();

		//@NonAction 에 설정된 Montage 가 실행되는지,
		bool IsPlayingNonAction = AnimInst->Montage_IsPlaying(NonActionMon);

		//@JumpSection - 아직 ApplyTime 이 남아있다면,
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
		//@ApplyTime 이 지났다면,
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

void UCUpset_Stun::ConditionOverlap(UCBaseConditionType * OverlappedCondition)
{
	Super::ConditionOverlap(OverlappedCondition);
	check(OverlappedCondition);

	//@초기화
	ApplyTime = OverlappedCondition->ApplyTime;
	InitUIColorAndOpacity();
}

void UCUpset_Stun::SetMontage(UAnimMontage * Montage)
{
	check(Montage);
	NonActionMon = Montage;
}
