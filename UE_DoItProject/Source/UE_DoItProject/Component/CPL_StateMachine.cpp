#include "CPL_StateMachine.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Charactor/Player/CPlayer.h"

#include "Component/Player/CPL_MageAttackComp.h"
#include "Component/Player/CPL_SwordAttackComp.h"
#include "Component/Player/CPL_MageActionComp.h"
#include "Component/Player/CPL_SwordActionComp.h"
#include "State/Player/CPL_ActionPullActorWithCable.h"


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
		strPath = L"AnimMontage'/Game/_Mine/Montages/Player/Common/CommonMon_CustomDash_Montage.CommonMon_CustomDash_Montage'";
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

	#pragma region Dash Niagara Particle

	strPath = L"NiagaraSystem'/Game/_Mine/UseParticle/Nia/Nia_AfterTest.Nia_AfterTest'";
	//Dash_NiaAfterImage = LoadObject<UNiagaraSystem>(NULL, *strPath);
	//UNiagaraFunctionLibrary::
	//ConstructorHelpers::FObjectFinder<UNiagaraSystem> DashNia(*strPath);
	//if (DashNia.Succeeded())
	//{
	//	Dash_NiaAfterImage = DashNia.Object;
	//}

	//UGameplayStatics::object

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

	#pragma region Create PullActorWithCable Action
	// Create PullActorWithCable Action
	{
		PullActorAction = CreateDefaultSubobject<UCPL_ActionPullActorWithCable>("ActionPullActor");
		//@Set OwnerPawn 해줘야함.
		PullActorAction->SetOwnerPawn(Cast<APawn>(GetOwner()));
	}

	#pragma endregion

	#pragma region Create Niagara Component
	{
		//NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("NiaComponent");
	}
	#pragma endregion

	// Setting
	Player = Cast<ACPlayer>(GetOwner());
}


void UCPL_StateMachine::BeginPlay()
{
	Super::BeginPlay();

	//@Set NiagaraComp Attach
	{
		//NiagaraComp->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
		//NiagaraComp->SetAsset(Dash_NiaAfterImage);
		//NiagaraComp->bAutoManageAttachment = true;
		//NiagaraComp->SetAutoAttachmentParameters(Player->GetMesh(), NAME_None, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative);
	}

	// @MageState 로 전환될 때, CastMontage 실행.
	OnMageState.AddLambda([&]()
	{
		Player->ActorAnimMonPlay(MageStateCastMontage, 1.5f, false);
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

	return nullptr;
}

// Swap State
// @ State 가 Mage, Sword 두개 밖에 존재하지 않아,
// 번갈아가며 Type 이 바뀌도록 내부적으로 ++Type 해줌.
// @I_StateManager의 (On/Un MageState), (On/Un SwordState) Delegate
void UCPL_StateMachine::OnSwapState()
{
	// @IF TRUE RETURN
	IfTrueRet(Player->IsJumping()); //@Jump Check
	IfTrueRet(Player->GetCharacterMovement()->IsFalling()); //@IsFalling - JUMP
	IfTrueRet(Player->GetIEquipComp()->GetEquiping()); //@Equping Check

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////

	// @Mage State
	if (CurrentStateType == EPlayerStateType::MAGE)
	{
		UnMageState.Broadcast();
	}
	// @Sword State
	else if (CurrentStateType == EPlayerStateType::SWORD)
	{
		UnSwordState.Broadcast();
	}

	int Type = static_cast<int>(CurrentStateType);
	++Type;
	(Type == static_cast<int>(EPlayerStateType::END))
		? CurrentStateType = static_cast<EPlayerStateType>(0)
		: CurrentStateType = static_cast<EPlayerStateType>(Type);

	// @Mage State
	if (CurrentStateType == EPlayerStateType::MAGE)
	{
		OnMageState.Broadcast();
	}
	// @Sword State
	else if (CurrentStateType == EPlayerStateType::SWORD)
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
	Player->SetEvadeSpeed(20.0f);

	APlayerController* Controller = Cast<APlayerController>(Player->GetController());
	if (Controller != nullptr)
	{
		FRotator ControlRotation = Player->GetControlRotation();
		FRotator Yaw = FRotator(0.0f, ControlRotation.Yaw, 0.0f);
		FQuat ControlQuat = FQuat(Yaw);
		if (Controller->IsInputKeyDown(EKeys::W))
		{
			SetAngleWithControlRot(0.0f);
			Player->SetEvadeDirection(ControlQuat.GetForwardVector());

			Player->OffCollision();
			Player->ActorAnimMonPlay(DashMontages[0], 1.0f, true);
		}
		else if (Controller->IsInputKeyDown(EKeys::S))
		{
			SetAngleWithControlRot(180.0f);
			Player->SetEvadeDirection((-1) * ControlQuat.GetForwardVector());

			Player->OffCollision();
			Player->ActorAnimMonPlay(DashMontages[0], 1.0f, true);
		}
		else if (Controller->IsInputKeyDown(EKeys::A))
		{
			SetAngleWithControlRot(-90.0f);
			Player->SetEvadeDirection((-1) * ControlQuat.GetRightVector());

			Player->OffCollision();
			Player->ActorAnimMonPlay(DashMontages[0], 1.0f, true);
		}
		else if (Controller->IsInputKeyDown(EKeys::D))
		{
			SetAngleWithControlRot(90.0f);
			Player->SetEvadeDirection(ControlQuat.GetRightVector());

			Player->OffCollision();
			Player->ActorAnimMonPlay(DashMontages[0], 1.0f, true);
		}
	}

	//@잔상 효과 - NiaComp(WeakPtr)
	//NiaComp_Dash->Activate(true);
}

/* Cable 을 사용해서 Actor 끌어오는 Action */
void UCPL_StateMachine::OnPullActorWithCable()
{
	PullActorAction->OnAction();
}

void UCPL_StateMachine::SetAngleWithControlRot(float Angle)
{
	FRotator ControlRotation = Player->GetControlRotation();
	FRotator Y = FRotator(0.0f, ControlRotation.Yaw + Angle, 0.0f);
	FVector Right = FQuat(Y).GetRightVector();

	Player->SetActorRotation(Y);
}
