#include "CPL_StateMachine.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Charactor/Player/CPlayer.h"

#include "Component/Base/C_BaseAttackComp.h"
#include "Component/Base/C_BaseActionComp.h"

#include "Component/Player/CPL_MageAttackComp.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "Component/Player/CPL_MageActionComp.h"
#include "Component/Player/CPL_SwordActionComp.h"


UCPL_StateMachine::UCPL_StateMachine()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString strPath = L"";
	#pragma region Mage Swap Montage
	// State Mage Swap Montage
	{
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MG_CastSwapState.MG_CastSwapState'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> cast(*strPath);
		if (cast.Succeeded())
			MageStateCastMontage = cast.Object;
	}
	#pragma endregion

	#pragma region Dash Montage
	// Dash Montage
	{
		UAnimMontage* F_DashMontage = nullptr;
		UAnimMontage* B_DashMontage = nullptr;
		UAnimMontage* L_DashMontage = nullptr;
		UAnimMontage* R_DashMontage = nullptr;

		// @Forward
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Common/CommonMon_Dash_F.CommonMon_Dash_F'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> F_Dash(*strPath);
		if (F_Dash.Succeeded())
			F_DashMontage = F_Dash.Object;

		// @Back
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Common/CommonMon_Dash_B.CommonMon_Dash_B'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> B_Dash(*strPath);
		if (B_Dash.Succeeded())
			B_DashMontage = B_Dash.Object;

		// @Left
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Common/CommonMon_Dash_L.CommonMon_Dash_L'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> L_Dash(*strPath);
		if (L_Dash.Succeeded())
			L_DashMontage = L_Dash.Object;

		// @Right
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Common/CommonMon_Dash_R.CommonMon_Dash_R'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> R_Dash(*strPath);
		if (R_Dash.Succeeded())
			R_DashMontage = R_Dash.Object;

		DashMontages.Emplace(F_DashMontage);
		DashMontages.Emplace(B_DashMontage);
		DashMontages.Emplace(L_DashMontage);
		DashMontages.Emplace(R_DashMontage);
	}

	#pragma endregion

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

	// Setting
	Player = Cast<ACPlayer>(GetOwner());
}


void UCPL_StateMachine::BeginPlay()
{
	Super::BeginPlay();

	// @MageState �� ��ȯ�� ��, CastMontage ����.
	OnMageState.AddLambda([&]()
	{
		Player->ActorAnimMonPlay(MageStateCastMontage, 1.5f, false);
	});
}


void UCPL_StateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* ���� StateType �� �ش��ϴ� 'Attack' Interface �� ������*/
IIC_AttackComp * UCPL_StateMachine::GetIAttackComp()
{
	int Type = static_cast<int>(CurrentStateType);
	return Cast<IIC_AttackComp>(AttackCompArray[Type]);
}

/* ���� StateType �� �ش��ϴ� 'Action' Interface �� ������*/
IIC_ActionComp * UCPL_StateMachine::GetIActionComp()
{
	int Type = static_cast<int>(CurrentStateType);
	return Cast<IIC_ActionComp>(ActionCompArray[Type]);
}

// Swap State
// @ State �� Mage, Sword �ΰ� �ۿ� �������� �ʾ�,
// �����ư��� Type �� �ٲ�� ���������� ++Type ����.
// @I_StateManager�� (On/Un MageState), (On/Un SwordState) Delegate
void UCPL_StateMachine::OnSwapState()
{
	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////

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

void UCPL_StateMachine::OnDash()
{
	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); // @Falling Check
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////////

	// @Set Evade
	Player->SetEvade(true);

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (Controller != nullptr)
	{
		FRotator ControlRotation = Player->GetControlRotation();
		FRotator Yaw = FRotator(0.0f, ControlRotation.Yaw, 0.0f);
		FQuat ControlQuat = FQuat(Yaw);

		Player->SetEvadeSpeed(10.0f);
		if (Controller->IsInputKeyDown(EKeys::W))
		{
			SetAngleWithControlRot(0.0f);
			Player->SetEvadeDirection(ControlQuat.GetForwardVector());

			Player->ActorAnimMonPlay(DashMontages[0], 1.0f, true);
		}
		else if (Controller->IsInputKeyDown(EKeys::S))
		{
			SetAngleWithControlRot(180.0f);
			Player->SetEvadeDirection((-1) * ControlQuat.GetForwardVector());

			Player->ActorAnimMonPlay(DashMontages[0], 1.0f, true);
		}
		else if (Controller->IsInputKeyDown(EKeys::A))
		{
			SetAngleWithControlRot(0.0f);
			Player->SetEvadeDirection((-1) * ControlQuat.GetRightVector());

			Player->ActorAnimMonPlay(DashMontages[2], 1.0f, true);
		}
		else if (Controller->IsInputKeyDown(EKeys::D))
		{
			SetAngleWithControlRot(0.0f);
			Player->SetEvadeDirection(ControlQuat.GetRightVector());

			Player->ActorAnimMonPlay(DashMontages[3], 1.0f, true);
		}
	}
}

void UCPL_StateMachine::SetAngleWithControlRot(float Angle)
{
	FRotator ControlRotation = Player->GetControlRotation();
	FRotator Y = FRotator(0.0f, ControlRotation.Yaw + Angle, 0.0f);
	FVector Right = FQuat(Y).GetRightVector();

	Player->SetActorRotation(Y);
}
