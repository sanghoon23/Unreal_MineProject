#include "C_BaseHitComp.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Interface/IC_Charactor.h"
#include "Interface/IC_Monster.h"
#include "DamageType/Base/CDamageType_Base.h"

UC_BaseHitComp::UC_BaseHitComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	//@Init
	{
		uint8 DamageTypeNum = static_cast<int>(FDamageType::END);
		bUsingDamageTypeEffect.Init(false, DamageTypeNum); //@DamageType 사용여부
		DamagedMontages.Init(nullptr, DamageTypeNum); //@DamageType 에 따른 Montage Container

		//DamagedMontages.SetNum(DamageTypeNum);

		CharactorMeshArray.Init(nullptr, static_cast<int>(ECharactorMeshSort::END));
	}
}


void UC_BaseHitComp::BeginPlay()
{
	Super::BeginPlay();

	I_Charactor = Cast<IIC_Charactor>(GetOwner());
	check(I_Charactor);
}

void UC_BaseHitComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn);

	//@죽었다면 Return
	IIC_Charactor* OwnerCharactor = Cast<IIC_Charactor>(OwnerPawn);
	if (OwnerCharactor != nullptr)
	{
		if (OwnerCharactor->IsDeath() == true)
		{
			for (int i = 0; i < ConditionDatas.Num(); ++i)
			{
				ConditionDatas[i]->EndCondition(OwnerPawn);
			}
			ConditionDatas.Empty();
			return;
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//@Apply 시간 다 되면 제거
	TArray<int> DeleteIndex;
	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		ConditionDatas[i]->UpdateCondition(OwnerPawn, DeltaTime);
		if (ConditionDatas[i]->ApplyTime < 0.0f)
		{
			ConditionDatas[i]->EndCondition(OwnerPawn);
			DeleteIndex.Add(i);
		}
	}

	//@제거
	for (int Index : DeleteIndex)
	{
		ConditionDatas.RemoveAt(Index);
	}
}

void UC_BaseHitComp::OnHit(AActor * AttackingActor, UCDamageType_Base * const DamageType, float DamageAmount)
{
	Attacker = AttackingActor;
}

void UC_BaseHitComp::RunMontageFromAttackType(EComboOrNot CanCombo, uint8 TypeNum, float MonSpeed, bool bAlways)
{
	if (CanCombo == EComboOrNot::NONE)
	{
		const uint8 MontageNum = TypeNum;
		if (MontageNum >= DamagedMontages.Num())
		{
			UE_LOG(LogTemp, Warning, L"HitComp MontageNumber EXCEED!!");
			return;
		}

		UAnimMontage* const RunMontage = DamagedMontages[MontageNum];
		if (RunMontage != nullptr)
		{
			I_Charactor->ActorAnimMonPlay(RunMontage, MonSpeed, bAlways);
		}
		else UE_LOG(LogTemp, Warning, L"BaseHitComp RunMontage NULL!!");
	}
	else
	{
		if (HitComboMon != nullptr)
		{
			SetCanHittedCombo(false);
			I_Charactor->ActorAnimMonPlay(HitComboMon, MonSpeed, bAlways);
		}
		else UE_LOG(LogTemp, Warning, L"BaseHitComp HitComboMon NULL!!");
	}
}

bool UC_BaseHitComp::AddConditionData(UCBaseConditionType* ConditionData)
{
	check(ConditionData);
	if (ConditionData == nullptr) return false;

	// @상태 이상 ADD
	UCBaseConditionType* const UpsetData = ConditionData;

	EHitUpset DataState = UpsetData->GetState();
	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		if (DataState == ConditionDatas[i]->GetState())
		{
			//@중첩 처리 - ( 기존 객체 에 UpsetData 를 집어넣음 )
			ConditionDatas[i]->ConditionOverlap(UpsetData);
			return true; //@Ret
		}
	}

	//@다 돌았는데도 없다면,
	UpsetData->StartCondition(Cast<APawn>(GetOwner()));
	ConditionDatas.Add(UpsetData);
	return true; //@Ret
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//@Get, Set


void UC_BaseHitComp::SettingCustomCharactorMesh(ECharactorMeshSort MeshSort, bool bNoneRestartAnimation)
{
	ACharacter* Charactor = Cast<ACharacter>(GetOwner());
	check(Charactor);

	const uint8 SortNum = static_cast<uint8>(MeshSort);
	if (SortNum >= CharactorMeshArray.Num() || SortNum < 0)
	{
		UE_LOG(LogTemp, Warning, L"SettingCustomCharactorMesh Func SortNum Exccess!!");
		CLog::Print(L"CustomMesh SortNum Exccess!!");
		return;
	}
	if (CharactorMeshArray[SortNum] == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"SettingCustomCharactorMesh Func SortNum NULLPTR!!");
		CLog::Print(L"CustomMesh NULLPTR!!");
		return;
	}

	CLog::Print(L"Call CustomMesh IN!!");

	if (bNoneRestartAnimation == false)
	{
		Charactor->GetMesh()->SetSkeletalMeshWithoutResettingAnimation
		(
			CharactorMeshArray[SortNum]
		);
	}
	else
	{
		Charactor->GetMesh()->SetSkeletalMesh
		(
			CharactorMeshArray[SortNum]
		);
	}
}

UParticleSystem * UC_BaseHitComp::GetBurnParticleOrNull() const
{
	if (BurnParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp BurnParticleComp NOT SETTING!!");
		return nullptr;
	}

	return BurnParticle;
}

UParticleSystem * UC_BaseHitComp::GetFreezeParticleOrNull() const
{
	if (FreezeParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp FreezeParticleComp NOT SETTING!!");
		return nullptr;
	}

	return FreezeParticle;
}

UParticleSystem * UC_BaseHitComp::GetStunHeadParticleOrNull() const
{
	if (StunHeadParticle == nullptr)
	{
		UE_LOG(LogTemp, Warning, L"BaseHitComp StunHeadParticle NOT SETTING!!");
		return nullptr;
	}

	return StunHeadParticle;
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

bool UC_BaseHitComp::IsEmptyConditionArray() const
{
	if (ConditionDatas.Num() == 0) return true;
	else return false;
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

void UC_BaseHitComp::GetConditionDatasOutArray(TArray<UCBaseConditionType*>* OutDataArray, int Count)
{
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

void UC_BaseHitComp::GetConditionDatasFromIndex(TArray<UCBaseConditionType*>* OutDataArray, int Index)
{
	if (ConditionDatas.Num() <= 0) return;
	if (Index >= ConditionDatas.Num() || Index < 0) return;

	(*OutDataArray)[Index] = ConditionDatas[Index];
}

bool UC_BaseHitComp::GetApplyUpsetStateInContainer(EHitUpset Sort, int & IndexCount)
{
	for (int i = 0; i < ConditionDatas.Num(); ++i)
	{
		if (ConditionDatas[i]->GetState() == Sort)
		{
			IndexCount = i;
			return true;
		}
	}

	return false;
}
