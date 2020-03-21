#include "C_BaseHitComp.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"

UC_BaseHitComp::UC_BaseHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UC_BaseHitComp::BeginPlay()
{
	Super::BeginPlay();
}

void UC_BaseHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//@ '�����̻�' ���� ������Ʈ - ( UpsetConditionDatas )
	// ...

	//@ '�ൿ�Ұ�' ���� ������Ʈ - ( NonActionConditionData )
	if (bNonAction == true)
	{
		float Time = NonActionConditionData.ApplyTime;
		Time -= DeltaTime;
		NonActionConditionData.ApplyTime = Time;

		//CLog::Print(Time);

		UAnimMontage* NonActionMontage = NonActionConditionData.NonActionMon;
		ACharacter* Charactor = Cast<ACharacter>(GetOwner());
		if (Charactor != nullptr)
		{
			UAnimInstance* AnimInst = Charactor->GetMesh()->GetAnimInstance();
			check(AnimInst);

			//@�ٸ� ��Ÿ�ְ� ����ǰ� �ִ���
			bool IsOtherMonPlaying = AnimInst->IsAnyMontagePlaying();

			//@NonAction �� ������ Montage �� ����Ǵ���,
			bool IsPlayingNonAction = AnimInst->Montage_IsPlaying(NonActionMontage);

			//@JumpSection - ���� ApplyTime �� �����ִٸ�,
			IIC_Monster* I_Monster = Cast<IIC_Monster>(Charactor);
			if (Time > 0.0f && IsOtherMonPlaying == false)
			{
				//@AI OFF
				I_Monster->SetAIRunningPossible(false);

				//@RUN Montage
				IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
				I_Charactor->ActorAnimMonPlay(NonActionMontage, 0.6f, true);
				AnimInst->Montage_JumpToSection
				(
					FName("Looping"), NonActionMontage //@Looping
				);
			}
			//@ApplyTime �� �����ٸ�,
			else if (Time <= 0.0f)
			{
				if (IsPlayingNonAction == true)
				{
					AnimInst->Montage_Stop(0.2f, NonActionMontage);
				}
				// @bool - NON ACTION
				bNonAction = false;

				//@AI ON
				I_Monster->SetAIRunningPossible(true);
			}
		}//(Charactor != nullptr)
	}//(bNonAction == true)
}

void UC_BaseHitComp::AddUpsetCondition(const FHitUpsetConditionData & ConditionData)
{
	//@ADD
	UpsetConditionDatas.Add(ConditionData);
}

void UC_BaseHitComp::AddNonActionCondition(const FHitNonActionConditionData& ConditionData)
{
	//@bool - NON ACTION
	bNonAction = true;

	//@Set
	NonActionConditionData = ConditionData;
}
