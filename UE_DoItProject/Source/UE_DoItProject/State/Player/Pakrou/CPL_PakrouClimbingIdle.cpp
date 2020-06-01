#include "CPL_PakrouClimbingIdle.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/CInteractActor_Climb.h"

UCPL_PakrouClimbingIdle::UCPL_PakrouClimbingIdle()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// Set ClimbingIdle Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/PakrouMon_Climbing_Idle.PakrouMon_Climbing_Idle'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbingIdleAction(*Path);
		if (ClimbingIdleAction.Succeeded())
			ClimbingIdleMontage = ClimbingIdleAction.Object;
	}
}

void UCPL_PakrouClimbingIdle::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);
}

void UCPL_PakrouClimbingIdle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// @Warning - SetCurrentBaseAction Setting
void UCPL_PakrouClimbingIdle::OnAction()
{
////////////////////////////////////////////////////////////////////////////////////////////////

	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @��Ÿ�� ����
	Player->ActorAnimMonPlay(ClimbingIdleMontage, 1.2f, true);
}

// @Warning -
// �߷� �� ���� �ٷ� �ӵ��� �ٿ��־�� ��
// ���� Action �� ������ �� OnBlockKeyInput,
// Bind�� ���� �ƴ�,  KeyDown �� ���� �ൿ�� ����
void UCPL_PakrouClimbingIdle::BeginActionState()
{
	Super::BeginActionState();

	// @�߷� ����
	Player->OffGravity();

	// @�ӷ� ���̱� - �߷²��� �ٷ� �������
	Player->GetCharacterMovement()->Velocity = FVector(0.0f);

	// @�̵� �Ұ�
	Player->CanNotMove();

	// @Warning - KeyInput �Ұ�
	Player->OnBlockKeyInput();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max ���ϱ�
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Z;

		// 3. �Ÿ� ���߱�
		SetTargetDistance();

		// 4. ���� ���߱�
		FVector StaticMeshLocation = ClimbActor->GetStaticMeshComponent()->GetComponentLocation();
		FVector ActorLocation = Player->GetActorLocation();
		ActorLocation.Z = (StaticMeshLocation.Z + (Max.Z * MeshScale) - 90.0f);

		// 5. Setting
		Player->SetActorLocation(ActorLocation);
	}
	else
		UE_LOG(LogTemp, Warning, L"PakrouClimbingIdle 'Begin' - ClimbActor NULL");
}

/* Pakrou ���ӵ� ���� Check Tick */
// @Warning -
// ClimbingIdle �� Climbing �� �⺻ ���� ��Ʈ.
// ����, KeyDown(S) �� �������� ��, �ݵ�� OffBlockKeyInput �� ���־����.
void UCPL_PakrouClimbingIdle::TickActionState()
{
	Super::TickActionState();
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Convert
	APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
	if (PlayerController != nullptr)
	{
		// @Stop
		if(PlayerController->IsInputKeyDown(EKeys::S))
		{
			//////////////////////////////////////////////////////////
			// @KeyInput Release
			Player->OffBlockKeyInput();
			//////////////////////////////////////////////////////////

			// @Stop Montage
			Player->ActorStopCurrentAnimMon();
		}
		// @ClimbOver
		else if (PlayerController->IsInputKeyDown(EKeys::W))
		{
			Player->OffHandIK(0);
			Player->OffHandIK(1);
			PakrouActionComp->DoLinkAction(PakrouLinkActionType::CLIMBOVER, PakrouObject);
		}
		// @Left Move
		else if (PlayerController->IsInputKeyDown(EKeys::A))
		{
			Player->OffHandIK(0);
			Player->OffHandIK(1);
			PakrouActionComp->DoLinkAction(PakrouLinkActionType::LEFT, PakrouObject);
		}
		// @Right Move
		else if (PlayerController->IsInputKeyDown(EKeys::D))
		{
			Player->OffHandIK(0);
			Player->OffHandIK(1);
			PakrouActionComp->DoLinkAction(PakrouLinkActionType::RIGHT, PakrouObject);
		}
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Mesh ���������� ����ǰ� �ϱ�

	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max ���ϱ�
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Z;
	}
}

void UCPL_PakrouClimbingIdle::EndActionState()
{
	Super::EndActionState();

	// @�߷� �ѱ�
	Player->OnGravity();

	// @�̵� ����
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}

