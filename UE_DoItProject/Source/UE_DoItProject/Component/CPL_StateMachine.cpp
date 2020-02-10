#include "CPL_StateMachine.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Charactor/Player/CPlayer.h"
#include "Component/Player/CPL_MageAttackComp.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "Component/Player/CPL_MageActionComp.h"
#include "Component/Player/CPL_SwordActionComp.h"


UCPL_StateMachine::UCPL_StateMachine()
{
	PrimaryComponentTick.bCanEverTick = true;

	// State Cast Montage
	FString strPath;
	{
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MG_CastSwapState.MG_CastSwapState'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> cast(*strPath);
		if (cast.Succeeded())
			MageStateCastMontage = cast.Object;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	#pragma region Create AttackComp
	// Create AttackComp
	{
		// @MageAttack
		UC_BaseAttackComp* MageAttack = CreateDefaultSubobject<UCPL_MageAttackComp>("MageAttack");

		// @SwordAttack
		UC_BaseAttackComp* SwordAttack = CreateDefaultSubobject<UCPL_SwordAttackComp>("SwordAttack");

		AttackCompArray.Emplace(MageAttack);
		AttackCompArray.Emplace(SwordAttack);
	}
	#pragma endregion

	#pragma region Create & AddAction State - Action Comp
	// Create Action Comp
	{
		// @Mage - 0
		UC_BaseActionComp* MageAction = CreateDefaultSubobject<UCPL_MageActionComp>("MageAction");
		ActionCompArray.Emplace(MageAction);

		// @Sword - 1
		UC_BaseActionComp* SwordAction = CreateDefaultSubobject<UCPL_SwordActionComp>("SwordAction");
		ActionCompArray.Emplace(SwordAction);
	}
	#pragma endregion

	Charactor = Cast<IIC_Charactor>(GetOwner());
}


void UCPL_StateMachine::BeginPlay()
{
	Super::BeginPlay();

	// @MageState 로 전환될 때, CastMontage 실행.
	OnMageState.AddLambda([&]()
	{
		Charactor->ActorAnimMonPlay(MageStateCastMontage, 1.5f, false);
	});
}


void UCPL_StateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* 현재 StateType 에 해당하는 'Attack' Interface 를 가져옴*/
IIC_AttackComp * UCPL_StateMachine::GetIAttackComp()
{
	int Type = static_cast<int>(CurrentStateType);
	return Cast<IIC_AttackComp>(AttackCompArray[Type]);
}

/* 현재 StateType 에 해당하는 'Action' Interface 를 가져옴*/
IIC_ActionComp * UCPL_StateMachine::GetIActionComp()
{
	int Type = static_cast<int>(CurrentStateType);
	return Cast<IIC_ActionComp>(ActionCompArray[Type]);
}

// Swap State
// @ State 가 Mage, Sword 두개 밖에 존재하지 않아,
// 번갈아가며 Type 이 바뀌도록 내부적으로 ++Type 해줌.
// @I_StateManager의 (On/Un MageState), (On/Un SwordState) Delegate
void UCPL_StateMachine::OnSwapState()
{
	ACPlayer* Player = Cast<ACPlayer>(GetOwner());
	check(Player);

	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

	// @Mage State
	if (CurrentStateType == PlayerStateType::MAGE)
	{
		UnMageState.Broadcast();
	}
	// @Sword State
	else if (CurrentStateType == PlayerStateType::SWORD)
	{
		UnSwordState.Broadcast();
	}

	int Type = static_cast<int>(CurrentStateType);
	++Type;
	(Type == static_cast<int>(PlayerStateType::END))
		? CurrentStateType = static_cast<PlayerStateType>(0)
		: CurrentStateType = static_cast<PlayerStateType>(Type);

	// @Mage State
	if (CurrentStateType == PlayerStateType::MAGE)
	{
		OnMageState.Broadcast();
	}
	// @Sword State
	else if (CurrentStateType == PlayerStateType::SWORD)
	{
		OnSwordState.Broadcast();
	}
}

