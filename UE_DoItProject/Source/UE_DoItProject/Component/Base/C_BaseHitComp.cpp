#include "C_BaseHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "DamageType/Base/CDamageType_Base.h"

UC_BaseHitComp::UC_BaseHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	//@Set MontageArrayNum
	uint8 DamageTypeNum = static_cast<int>(FDamageType::END);
	DamagedMontages.Init(nullptr, DamageTypeNum);
	//DamagedMontages.SetNum(DamageTypeNum);
}


void UC_BaseHitComp::BeginPlay()
{
	Super::BeginPlay();
}

void UC_BaseHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn);

	////@�׾��ٸ� Return
	//IIC_Charactor* OwnerCharactor = Cast<IIC_Charactor>(OwnerPawn);
	//if (OwnerCharactor != nullptr)
	//{
	//	IfTrueRet(OwnerCharactor->IsDeath() == true);
	//}

	//@Apply �ð� �� �Ǹ� ����
	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		ConditionDatas[i]->UpdateCondition(OwnerPawn, DeltaTime);
		if (ConditionDatas[i]->ApplyTime < 0.0f)
		{
			ConditionDatas[i]->EndCondition(OwnerPawn);
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
	UpsetData->StartCondition(Cast<APawn>(GetOwner()));
	ConditionDatas.Add(UpsetData);
	return true; //@Ret
}

UMaterialInterface * UC_BaseHitComp::GetPoisionMaterialOrNull() const
{
	if (PoisionMaterial == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp PoisionMaterial NOT SETTING!!");
		return nullptr;
	}
	return PoisionMaterial;
}

UParticleSystemComponent * UC_BaseHitComp::GetBurnParticleCompOrNull() const
{
	if (BurnParticleComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp BurnParticleComp NOT SETTING!!");
		return nullptr;
	}

	return BurnParticleComp;
}

UParticleSystemComponent * UC_BaseHitComp::GetFreezeParticleCompOrNull() const
{
	if (FreezeParticleComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp FreezeParticleComp NOT SETTING!!");
		return nullptr;
	}

	return FreezeParticleComp;
}

UAnimMontage * UC_BaseHitComp::GetDamagedMontageOrNull(const uint8 ArrayNum)
{
	if (ArrayNum >= DamagedMontages.Num())
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp GetDamagedMontage ArrayNumber Overflow");
		return nullptr;
	}

	return DamagedMontages[ArrayNum];
}

UCBaseConditionType * UC_BaseHitComp::GetConditionData(int Index)
{
	if (Index >= ConditionDatas.Num() || Index < 0)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp GetConditionData ArrayNumber Overflow");
		return nullptr;
	}

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
