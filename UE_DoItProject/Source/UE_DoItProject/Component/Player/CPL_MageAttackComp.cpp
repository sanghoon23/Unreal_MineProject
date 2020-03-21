#include "CPL_MageAttackComp.h"
#include "Global.h"

#include "State/Player/Base/CPL_MageBaseAttack.h"
#include "State/Player/Mage/CPL_MGAttackBasic.h"

UCPL_MageAttackComp::UCPL_MageAttackComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	#pragma region Create State
	// Create State
	{
		UCPL_MageBaseAttack* MG_BasicAttack = CreateDefaultSubobject<UCPL_MGAttackBasic>("Mage_AttackFrist");
		MG_BasicAttack->SetOwnerPawn(Cast<APawn>(GetOwner()));
		MageAttackStateArray.Emplace(MG_BasicAttack);
	}
	#pragma endregion
}


void UCPL_MageAttackComp::BeginPlay()
{
	Super::BeginPlay();
}


void UCPL_MageAttackComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// - IC_AttackComp ����.
// @Ref
// @Type - �ٲ� AttackType �� �Ѱ���.
// #Edit *0220
// ���� Ÿ�԰� �ٲ��, EndAttack BroadCast ��, Ÿ�� ����, ������ Type IBaseAttack ����.
IIC_BaseAttack * UCPL_MageAttackComp::SetAttackTypeRetIBaseAttack(uint8 Type)
{
	IfTureRetResult
	(
		Type >= static_cast<uint8>(EMageAttackType::END), 
		nullptr
	);

	// @���� AttackType Num
	uint8 BeforeTypeNum = static_cast<uint8>(AttackType);

	// @���� Type
	EMageAttackType SetType = static_cast<EMageAttackType>(Type);
	if (AttackType == SetType)
	{
		return Cast<IIC_BaseAttack>(MageAttackStateArray[Type]);
	}
	else if (AttackType != SetType) // �ٲ���ٸ�
	{
		// @EndAttack Call
		MageAttackStateArray[BeforeTypeNum]->EndAttackDeleFunc.Broadcast();
		AttackType = SetType;
	}

	uint8 AfterTypeNum = static_cast<uint8>(AttackType);
	return Cast<IIC_BaseAttack>(MageAttackStateArray[Type]);
}

// - IC_AttackComp ����.
// @Type - AttackStateArray ���� ������ AttackType
// @Return - @Type �� �ش��ϴ� IBaseAttack
IIC_BaseAttack * UCPL_MageAttackComp::GetCurrentIBaseAttack()
{
	int CurrentType = static_cast<int>(AttackType);
	if (CurrentType > MageAttackStateArray.Num() - 1)
	{
		CLog::Print(L"MGAttackComp IBaseAttack Array Excess!!");
		return nullptr; /*@Return*/
	}


	return Cast<IIC_BaseAttack>(MageAttackStateArray[CurrentType]);
}

