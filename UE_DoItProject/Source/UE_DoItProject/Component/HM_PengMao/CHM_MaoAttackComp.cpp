#include "CHM_MaoAttackComp.h"
#include "Global.h"
#include "Interface/IC_Component.h"

#include "State/Base/C_BaseAttackState.h"
#include "State/HM_PengMao/CHM_MaoFirstAttack.h"
#include "State/HM_PengMao/CHM_MaoSecondAttack.h"
#include "State/HM_PengMao/CHM_MaoThirdAttack.h"
#include "State/HM_PengMao/CHM_MaoFourAttack.h"

UCHM_MaoAttackComp::UCHM_MaoAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	//BasicAttackStateArray.Init(nullptr, static_cast<int32>(HM_PengMaoAttackType::END));

	#pragma region Create State
	// @FirstCombo
	{
		UCHM_MaoFirstAttack* HMMao_FirstAttack = CreateDefaultSubobject<UCHM_MaoFirstAttack>(TEXT("FirstAttack_BaseAttac"));
		UCHM_MaoSecondAttack* HMMao_SecondAttack = CreateDefaultSubobject<UCHM_MaoSecondAttack>(TEXT("SecondAttack_FlyDow"));
		UCHM_MaoThirdAttack* HMMao_ThirdAttack = CreateDefaultSubobject<UCHM_MaoThirdAttack>(TEXT("ThridATK_S"));
		UCHM_MaoFourAttack* HMMao_Four = CreateDefaultSubobject<UCHM_MaoFourAttack>(TEXT("FourATK_A"));


		BasicAttackStateArray.Emplace(HMMao_FirstAttack);
		BasicAttackStateArray.Emplace(HMMao_SecondAttack);
		BasicAttackStateArray.Emplace(HMMao_ThirdAttack);
		BasicAttackStateArray.Emplace(HMMao_Four);
	}

	#pragma endregion
}

void UCHM_MaoAttackComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCHM_MaoAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp 참고.
// @Type - AttackStateArray 에서 가져올 AttackType
// @Return - @Type 에 해당하는 IBaseAttack
IIC_BaseAttack * UCHM_MaoAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTrueRetResult
	(
		Type >= static_cast<uint8>(EHM_PengMaoAttackType::END),
		nullptr
	);

	// @기존 AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @들어온 Type
	EHM_PengMaoAttackType SetType = static_cast<EHM_PengMaoAttackType>(Type);
	if (AttackType == SetType)
	{
		// @Tick true
		IIC_Component* IC_Comp = Cast<IIC_Component>(BasicAttackStateArray[Type]);
		if (IC_Comp != nullptr)
		{
			IC_Comp->IsRunTick(true);
		}

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
IIC_BaseAttack * UCHM_MaoAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > BasicAttackStateArray.Num() - 1)
	{
		CLog::Print(L"CHM_MaoATKComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}

	return Cast<IIC_BaseAttack>(BasicAttackStateArray[CurrentType]);
}

