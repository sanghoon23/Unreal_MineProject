#include "C_BaseHitComp.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"

UC_BaseHitComp::UC_BaseHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UC_BaseHitComp::BeginPlay()
{
	Super::BeginPlay();
}

void UC_BaseHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		float Time = ConditionDatas[i]->ApplyTime;
		Time -= DeltaTime;
		ConditionDatas[i]->ApplyTime = Time;

		//@UpsetCondition Update
		UHitUpsetConditionData* UpsetConditionData
			= Cast<UHitUpsetConditionData>(ConditionDatas[i]);
		if (UpsetConditionData != nullptr)
		{
			if (UpdateUpsetCondition(UpsetConditionData) == true)
			{
				ConditionDatas.RemoveAt(i);
			}
		}

		//@NonAction Update
		UHitNonActionConditionData* NonActionData 
			= Cast<UHitNonActionConditionData>(ConditionDatas[i]);
		if (NonActionData != nullptr)
		{
			if (UpdateNonActionCondition(NonActionData) == true)
			{
				ConditionDatas.RemoveAt(i);
			}
		}
	}
}

void UC_BaseHitComp::AddConditionData(UConditionData* ConditionData)
{
	ConditionDatas.Add(ConditionData);
}

bool UC_BaseHitComp::UpdateUpsetCondition(UHitUpsetConditionData * ConditionData)
{
	// TODO :
	return false;
}

bool UC_BaseHitComp::UpdateNonActionCondition(UHitNonActionConditionData * ConditionData)
{
	check(ConditionData);

	//@ '�ൿ�Ұ�'
	UHitNonActionConditionData* const NonActionData = ConditionData;
	if (NonActionData != nullptr)
	{
		UAnimMontage* NonActionMontage = NonActionData->NonActionMon;
		check(NonActionMontage);
		ACharacter* Charactor = Cast<ACharacter>(GetOwner());
		if (Charactor != nullptr)
		{
			UAnimInstance* AnimInst = Charactor->GetMesh()->GetAnimInstance();
			check(AnimInst);

			//@���� �ð� ���� ColorAndOpacity Update - (OpacityLinearTimer)
			if (NonActionData->ApplyTime < OpacityLinearTimer)
			{
				UpdateColorAndOpacity(NonActionData);
			}

			//@�ٸ� ��Ÿ�ְ� ����ǰ� �ִ���
			bool IsOtherMonPlaying = AnimInst->IsAnyMontagePlaying();

			//@NonAction �� ������ Montage �� ����Ǵ���,
			bool IsPlayingNonAction = AnimInst->Montage_IsPlaying(NonActionMontage);

			//@JumpSection - ���� ApplyTime �� �����ִٸ�,
			IIC_Monster* I_Monster = Cast<IIC_Monster>(Charactor);
			if (NonActionData->ApplyTime > 0.0f && IsOtherMonPlaying == false)
			{
				//@AI OFF
				I_Monster->SetAIRunningPossible(false);

				//@RUN Montage
				IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Charactor);
				if (I_Charactor != nullptr)
				{
					I_Charactor->ActorAnimMonPlay(NonActionMontage, 0.6f, true);
					AnimInst->Montage_JumpToSection
					(
						FName("Looping"), NonActionMontage //@Looping
					);
				}
			}
			//@ApplyTime �� �����ٸ�,
			else if (NonActionData->ApplyTime <= 0.0f)
			{
				if (IsPlayingNonAction == true)
				{
					AnimInst->Montage_Stop(0.5f, NonActionMontage);
				}

				//@AI ON
				I_Monster->SetAIRunningPossible(true);

				//@Return - (ApplyTime < 0.0f)
				return true;
			}
		}//(Charactor != nullptr)
	}//(NonActionData != nullptr)

	return false;
}

void UC_BaseHitComp::UpdateColorAndOpacity(UConditionData* ConditionData)
{
	float Opacity = ConditionData->ColorAndOpacity.A;

	(ConditionData->bLinerColorDir == true)
		? Opacity += OpacityLinearSpeed
		: Opacity -= OpacityLinearSpeed;

	if (Opacity > 1.0f || Opacity < 0.1f)
	{
		ConditionData->bLinerColorDir = !(ConditionData->bLinerColorDir);
	}

	ConditionData->ColorAndOpacity.A = Opacity;
}

UConditionData * UC_BaseHitComp::GetConditionData(int Index)
{
	if (Index >= ConditionDatas.Num() || Index < 0)
		return nullptr;

	return ConditionDatas[Index];
}

void UC_BaseHitComp::GetConditionDatasAfterEmpty(TArray<UConditionData*>* OutDataArray)
{
	//@Empty - OutTArray
	OutDataArray->Empty();

	if (ConditionDatas.Num() <= 0) return;
	for (UConditionData* Data : ConditionDatas)
	{
		OutDataArray->Push(Data);
	}
}

void UC_BaseHitComp::GetConditionDatasWithIndex(TArray<UConditionData*>* OutDataArray, int Index)
{
	if (ConditionDatas.Num() <= 0) return;
	if (Index >= ConditionDatas.Num() || Index < 0) return;

	(*OutDataArray)[Index] = ConditionDatas[Index];
}
