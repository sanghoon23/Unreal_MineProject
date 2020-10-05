#include "CHM_AssassinAttackComp.h"
#include "Global.h"
#include "Interface/IC_Component.h"

#include "State/Base/C_BaseAttackState.h"
#include "State/HM_Assassin/CHM_AssaFirstAttack.h"
#include "State/HM_Assassin/CHM_AssaSecondAttack.h"
#include "State/HM_Assassin/CHM_AssaThirdAttack.h"
#include "State/HM_Assassin/CHM_AssaFourAttack.h"
#include "State/HM_Assassin/CHM_AssaFiveAttack.h"
#include "State/HM_Assassin/CHM_AssaSixAttack.h"

UCHM_AssassinAttackComp::UCHM_AssassinAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	//BasicAttackStateArray.Init(nullptr, static_cast<int32>(HM_PengMaoAttackType::END));

#pragma region Create State
	{
		UC_BaseAttackState* HMAssa_FirstAttack = CreateDefaultSubobject<UCHM_AssaFirstAttack>(TEXT("Assa_FirstAttack"));
		UC_BaseAttackState* HMAssa_SecondAttack = CreateDefaultSubobject<UCHM_AssaSecondAttack>(TEXT("Assa_SecondAttack"));
		UC_BaseAttackState* HMAssa_ThirdAttack = CreateDefaultSubobject<UCHM_AssaThirdAttack>(TEXT("Assa_ThirdATK"));
		UC_BaseAttackState* HMAssa_FourAttack = CreateDefaultSubobject<UCHM_AssaFourAttack>(TEXT("Assa_FourATK"));
		UC_BaseAttackState* HMAssa_FiveAttack = CreateDefaultSubobject<UCHM_AssaFiveAttack>(TEXT("_FiveATK"));
		UC_BaseAttackState* HMAssa_SixAttack = CreateDefaultSubobject<UCHM_AssaSixAttack>(TEXT("_SixATK"));

		BasicAttackStateArray.Emplace(HMAssa_FirstAttack);
		BasicAttackStateArray.Emplace(HMAssa_SecondAttack);
		BasicAttackStateArray.Emplace(HMAssa_ThirdAttack);
		BasicAttackStateArray.Emplace(HMAssa_FourAttack);
		BasicAttackStateArray.Emplace(HMAssa_FiveAttack);
		BasicAttackStateArray.Emplace(HMAssa_SixAttack);
	}

#pragma endregion
}

void UCHM_AssassinAttackComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCHM_AssassinAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp 참고.
// @Type - AttackStateArray 에서 가져올 AttackType
// @Return - @Type 에 해당하는 IBaseAttack
IIC_BaseAttack * UCHM_AssassinAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTrueRetResult
	(
		Type >= static_cast<uint8>(EHM_AssassinAttackType::END),
		nullptr
	);

	// @기존 AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @들어온 Type
	EHM_AssassinAttackType SetType = static_cast<EHM_AssassinAttackType>(Type);
	if (AttackType == SetType)
	{
		//// @Tick true
		//IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[Type]);
		//if (IC_Comp != nullptr)
		//{
		//	IC_Comp->IsRunTick(true);
		//}

		return Cast<IIC_BaseAttack>(BasicAttackStateArray[Type]); //@return
	}
	else if (AttackType != SetType)
	{
		// @Tick false
		IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[BeforeTypeNum]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(false);
		}

		// @EndAttack Call
		BasicAttackStateArray[BeforeTypeNum]->EndAttackDeleFunc.Broadcast();

		AttackType = SetType;
	}

	uint8 AfterTypeNum = static_cast<uint8>(AttackType);

	// @Tick true
	IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[AfterTypeNum]);
	if (IC_Comp != nullptr)
	{
		IC_Comp->IsRunTick(true);
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[AfterTypeNum]);
}

// - IC_AttackComp 참고.
// @현재 설정된 AttackType 의 Interface_BaseAttack 을 가져옴.
IIC_BaseAttack * UCHM_AssassinAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > BasicAttackStateArray.Num() - 1)
	{
		CLog::Print(L"CHM_AssassinATKComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[CurrentType]);
}

void UCHM_AssassinAttackComp::GetBaseAttackList(TArray<class UC_BaseAttackState*>& Out)
{
	for (auto& BaseAttack : BasicAttackStateArray)
	{
		//IIC_BaseAttack* Insert = Cast<IIC_BaseAttack>(BaseAttack)
		Out.Add(BaseAttack);
	}
}

UC_BaseAttackState * UCHM_AssassinAttackComp::GetBaseAttack(uint8 Type)
{
	int TypeNum = static_cast<int>(Type);
	if (TypeNum > BasicAttackStateArray.Num() - 1)
	{
		CLog::Print(L"CHM_AssassinATKComp GetBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return BasicAttackStateArray[TypeNum];
}


