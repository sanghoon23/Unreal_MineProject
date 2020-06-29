#include "CPL_MGActionTeleprot.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "System/CS_MouseController.h"
#include "_FunctionLibrary/CFL_ActorAgainst.h"
#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"

UCPL_MGActionTeleprot::UCPL_MGActionTeleprot()
{
	PrimaryComponentTick.bCanEverTick = true;

	FString Path = L"";

	// Set Teleprot Action
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Mage/MEvade/MG_TeleProt.MG_Teleprot'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> TeleprotAction(*Path);
		if (TeleprotAction.Succeeded())
			TeleprotMontage = TeleprotAction.Object;
	}


	// Load Particle System
	{
		Path = L"ParticleSystem'/Game/_Mine/UseParticle/Charactor/P_MG_Teleprot.P_MG_Teleprot'";
		ConstructorHelpers::FObjectFinder<UParticleSystem> P_Teleport(*Path);
		if (P_Teleport.Succeeded())
			TeleportParticle = P_Teleport.Object;
	}
}

void UCPL_MGActionTeleprot::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);
	
	// @Set MouseController
	MouseController = Player->GetPlayerCSMouseController();
	check(MouseController);
}

void UCPL_MGActionTeleprot::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bStartWaiting == true)
	{
		//UCS_MouseController* MC = Player->GetPlayerCSMouseController();
		if (MouseController != nullptr)
		{
			EMouseState MCState = MouseController->GetMouseState();
			if (MCState == EMouseState::CHECKINGPOINT)
			{
				//@Play
				SetPlayAfterMouseControl();
				EndWaiting();
			}
			else if (MCState == EMouseState::NONE)
			{
				Player->OffBlockAction();
				EndWaiting();
			}
		}
	}

}

void UCPL_MGActionTeleprot::OnAction()
{
	Super::OnAction();

	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////////

	StartWaiting();
}

/* OnAction ��, MouseController ���� Player �� Ŭ���� ���� ���� ���� */
void UCPL_MGActionTeleprot::SetPlayAfterMouseControl()
{
	Player->ActorAnimMonPlay
	(
		TeleprotMontage, 1.3f, true
	);

	float MontageStartTime, MontageEndTime;
	TeleprotMontage->GetSectionStartAndEndTime(0, MontageStartTime, MontageEndTime);

	//@Delegate And SetTimer
	PlayHandle.BindUFunction(this, FName("PlayAfterMouseControl"));
	GetWorld()->GetTimerManager().SetTimer(EndMontageTimerHandle, PlayHandle, 1.0f, false, MontageEndTime);
}

/* MouseControlelr �� Waiting ���� */
void UCPL_MGActionTeleprot::StartWaiting()
{
	// @�̵� Ű�� ������ Action Key Block �ϱ�.
	Player->OnBlockAction();

	MouseController->OnUsingDecalMouseControl(FVector(100.0f), Player, 2500.0f);
	bStartWaiting = true;
}

/* MouseController �� Waiting �ߴ� */
void UCPL_MGActionTeleprot::EndWaiting()
{
	bStartWaiting = false;
}

/* TimerDelegate �� �� UFUCTION */
void UCPL_MGActionTeleprot::PlayAfterMouseControl()
{
	//@Visibility - ����
	Player->OffParticleInPlayer();
	Player->GetMesh()->SetVisibility(false);

	//@Particle Spawn - PlayerLocation
	{
		FVector PT_Position = Player->GetActorLocation();
		PT_Position.Z -= 20.0f;

		FTransform PT_Transform = FTransform::Identity;
		PT_Transform.SetScale3D(FVector(3.0f));
		PT_Transform.SetLocation(PT_Position);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportParticle, PT_Transform, true);
	}

	//@Position �޾ƿͼ�, SetPosition
	FVector ClickedPosition = MouseController->GetClickPoint();
	ClickedPosition.Z = 0.0f;
	ClickedPosition.Z += Player->GetDefaultHalfHeight();
	Player->SetActorLocation(ClickedPosition);

	//@Particle Spawn - ClickedPosition
	{
		FVector PT_Position = ClickedPosition;
		PT_Position.Z -= 20.0f;

		FTransform PT_Transform = FTransform::Identity;
		PT_Transform.SetScale3D(FVector(3.0f));
		PT_Transform.SetLocation(PT_Position);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportParticle, PT_Transform, true);
	}

	//@Off BlockAction
	Player->OffBlockAction();

	//@Visibility - ���� ����
	FTimerHandle T_Handle;
	GetWorld()->GetTimerManager().SetTimer(T_Handle, this, &UCPL_MGActionTeleprot::PlayerVisibleAfterParticle, 0.3f);
}

void UCPL_MGActionTeleprot::PlayerVisibleAfterParticle()
{
	Player->OnParticleInPlayer();
	Player->GetMesh()->SetVisibility(true);
}
