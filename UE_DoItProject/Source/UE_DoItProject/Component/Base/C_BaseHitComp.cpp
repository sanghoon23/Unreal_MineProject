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

	//@Apply �ð� �� �Ǹ� ����
	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		ConditionDatas[i]->UpdateConditionOnActor(GetOwner(), DeltaTime);
		if (ConditionDatas[i]->ApplyTime < 0.0f)
		{
			ConditionDatas[i]->EndConditionOnActor(GetOwner());
			ConditionDatas.RemoveAt(i);
		}
	}
}

bool UC_BaseHitComp::AddConditionData(UCBaseConditionType* ConditionData)
{
	check(ConditionData);
	if (ConditionData == nullptr) return false;

	// @���� �̻� ADD
	UCBaseConditionType* const UpsetData = ConditionData;

	EHitUpset DataState = UpsetData->GetState();
	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		if (DataState == ConditionDatas[i]->GetState())
		{
			//@��ø ó�� - ( ���� ��ü �� UpsetData �� ������� )
			ConditionDatas[i]->ConditionOverlap(UpsetData);
			return true; //@Ret
		}
	}

	//@�� ���Ҵµ��� ���ٸ�,
	UpsetData->StartConditionOnActor(GetOwner());
	ConditionDatas.Add(UpsetData);
	return true; //@Ret
}

UCBaseConditionType * UC_BaseHitComp::GetConditionData(int Index)
{
	if (Index >= ConditionDatas.Num() || Index < 0)
		return nullptr;

	return ConditionDatas[Index];
}

void UC_BaseHitComp::GetConditionDatasAfterEmpty(TArray<UCBaseConditionType*>* OutDataArray, int Count)
{
	//@Empty - OutTArray
	OutDataArray->Empty();

	const int DataArraySize = ConditionDatas.Num();

	int Size = 0;
	(Count == -1)
		? Size = DataArraySize
		: Size = Count;

	if (DataArraySize <= 0) return;
	if (DataArraySize <= Size)
	{
		Size = DataArraySize;
	}

	for (int i = 0; i < Size; ++i)
	{
		OutDataArray->Add(ConditionDatas[i]);
	}
}

void UC_BaseHitComp::GetConditionDatasByCount(TArray<UCBaseConditionType*>* OutDataArray, int IndexCount)
{
	if (ConditionDatas.Num() <= 0) return;
	if (IndexCount >= ConditionDatas.Num() || IndexCount < 0) return;

	for (int i = 0; i < IndexCount; ++i)
	{
		OutDataArray->Add(ConditionDatas[i]);
	}
}


void UC_BaseHitComp::GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index)
{
	if (ConditionDatas.Num() <= 0) return;
	if (Index >= ConditionDatas.Num() || Index < 0) return;

	(*OutDataArray)[Index] = ConditionDatas[Index];
}
