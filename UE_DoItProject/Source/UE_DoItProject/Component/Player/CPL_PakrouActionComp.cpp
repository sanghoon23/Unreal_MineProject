#include "CPL_PakrouActionComp.h"
#include "Global.h"

#include "State/Player/Pakrou/CPL_PakrouJumpOver.h"
#include "State/Player/Pakrou/CPL_PakrouClimbingIdle.h"
#include "State/Player/Pakrou/CPL_PakrouWallClimbRun.h"
#include "State/Player/Pakrou/CPL_PakrouClimbingLeftMove.h"
#include "State/Player/Pakrou/CPL_PakrouClimbingRightMove.h"
#include "State/Player/Pakrou/CPL_PakrouClimbOver.h"

UCPL_PakrouActionComp::UCPL_PakrouActionComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	// @Create
	UCPL_PakrouBaseAction* JumpOver = CreateDefaultSubobject<UCPL_PakrouJumpOver>("JumpOverAction");
	UCPL_PakrouBaseAction* ClimbingIdle = CreateDefaultSubobject<UCPL_PakrouClimbingIdle>("ClimbingIdleAction");
	UCPL_PakrouBaseAction* WallClimbRun = CreateDefaultSubobject<UCPL_PakrouWallClimbRun>("WallClimbRunAction");

	#pragma region Set StartBaseAction
	// Set StartBaseAciton
	{
		// @SetOwner
		JumpOver->SetOwnerPawn(Cast<APawn>(GetOwner()));
		ClimbingIdle->SetOwnerPawn(Cast<APawn>(GetOwner()));
		WallClimbRun->SetOwnerPawn(Cast<APawn>(GetOwner()));

		// @SetPakrouComp
		JumpOver->SetPakrouComponent(this);
		ClimbingIdle->SetPakrouComponent(this);
		WallClimbRun->SetPakrouComponent(this);

		// @Emplace
		StartActionArray.Emplace(JumpOver);
		StartActionArray.Emplace(ClimbingIdle);
		StartActionArray.Emplace(WallClimbRun);
	}
	#pragma endregion

	/*
	@Warning
	- ClimbingIdle �� StartAction, LinkAction ���� ��
	*/
	UCPL_PakrouBaseAction* ClimbingLeftMove = CreateDefaultSubobject<UCPL_PakrouClimbingLeftMove>("ClimbingLeftMoveAction");
	UCPL_PakrouBaseAction* ClimbingRightMove = CreateDefaultSubobject<UCPL_PakrouClimbingRightMove>("ClimbingRightMoveAction");
	UCPL_PakrouBaseAction* ClimbOver = CreateDefaultSubobject<UCPL_PakrouClimbOver>("ClimbOverAction");
	#pragma region Set PakrouLinkAction
	// Set PakrouLinkAction
	{
		// @SetOwner
		ClimbingLeftMove->SetOwnerPawn(Cast<APawn>(GetOwner()));
		ClimbingRightMove->SetOwnerPawn(Cast<APawn>(GetOwner()));
		ClimbOver->SetOwnerPawn(Cast<APawn>(GetOwner()));

		// @SetPakrouComp
		ClimbingLeftMove->SetPakrouComponent(this);
		ClimbingRightMove->SetPakrouComponent(this);
		ClimbOver->SetPakrouComponent(this);

		// @Emplace
		PakrouLinkAction.Emplace(ClimbingIdle);
		PakrouLinkAction.Emplace(ClimbingLeftMove);
		PakrouLinkAction.Emplace(ClimbingRightMove);
		PakrouLinkAction.Emplace(ClimbOver);
	}
	#pragma endregion
}

void UCPL_PakrouActionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_PakrouActionComp::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_PakrouActionComp::StartAction(PakrouStartActionType Type, AActor* Target)
{
	// StartActionArray[TypeNum]->SetComponentTickEnabled(true);

	uint8 TypeNum = static_cast<uint8>(Type);

	// @Object ����.
	StartActionArray[TypeNum]->SetPakrouObject(Target);

	// @IBaseAction ���� - StartAction
	StartActionArray[TypeNum]->OnAction();
}

void UCPL_PakrouActionComp::DoLinkAction(PakrouLinkActionType Type, AActor* Target)
{
	uint8 TypeNum = static_cast<uint8>(Type);

	// @Object ����.
	PakrouLinkAction[TypeNum]->SetPakrouObject(Target);

	// @IBaseAction ���� - PakrouLink
	PakrouLinkAction[TypeNum]->OnAction();
}
