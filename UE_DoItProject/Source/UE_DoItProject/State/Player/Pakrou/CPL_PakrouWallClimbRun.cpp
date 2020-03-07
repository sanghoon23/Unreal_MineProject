#include "CPL_PakrouWallClimbRun.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/CInteractActor_Climb.h"

UCPL_PakrouWallClimbRun::UCPL_PakrouWallClimbRun()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// Set WallClimbRun Montage
	{
		// @WallClimb
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/PakrouMon_WallClimbRun.PakrouMon_WallClimbRun'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> WallClimbAction(*Path);
		if (WallClimbAction.Succeeded())
			WallClimbRunMontage = WallClimbAction.Object;

		// @NextClimbingIdel
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/PakrouMon_Climbing_Idle.PakrouMon_Climbing_Idle'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> ClimbingIdleAction(*Path);
		if (ClimbingIdleAction.Succeeded())
			NextClimbingIdleMontage = ClimbingIdleAction.Object;
	}
}

void UCPL_PakrouWallClimbRun::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(OwnerPawn);
	check(Player);
}

void UCPL_PakrouWallClimbRun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// @Warning - SetCurrentBaseAction Setting
void UCPL_PakrouWallClimbRun::OnAction()
{
////////////////////////////////////////////////////////////////////////////////////////////////

	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @Montage ����
	Player->ActorAnimMonPlay(WallClimbRunMontage, 1.0f, true);
}

void UCPL_PakrouWallClimbRun::BeginActionState()
{
	Super::BeginActionState();

	// @�߷� ����
	Player->OffGravity();

	// @�ӷ� ���̱� - �߷²��� �ٷ� �������
	Player->GetCharacterMovement()->Velocity = FVector(0.0f);

	// @�̵� �Ұ�
	Player->CanNotMove();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max ���ϱ�
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Y;
		SetTargetDistance();
	}
	else
		UE_LOG(LogTemp, Warning, L"PakrouWallClimbRun 'Begin' - ClimbActor NULL");
}

void UCPL_PakrouWallClimbRun::TickActionState()
{
	Super::TickActionState();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// @Player Location
	FVector ActorLocation = Player->GetActorLocation();

	// @1. Up Location
	FVector ActorUpVec = Player->GetActorUpVector();
	ActorLocation += (ActorUpVec * WallClimbingSpeed);

	// @2. Climb Reach Check
	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max ���ϱ�
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Z;

		// 3. StaticMesh ���ϱ�.
		// @�� �ö󰬴ٸ�? -> NextAction ���� ( Climbing Idle )
		FVector StaticMeshLocation = ClimbActor->GetStaticMeshComponent()->GetComponentLocation();
		if (ActorLocation.Z > (StaticMeshLocation.Z + (Max.Z * MeshScale) - 90.0f))
		{
			Player->ActorStopCurrentAnimMon();
			PakrouActionComp->DoLinkAction(PakrouLinkActionType::IDLE, PakrouObject);
			return;
		}
	}
	else
		UE_LOG(LogTemp, Warning, L"PakrouWallClimbRun 'Tick' - ClimbActor NULL");

	// @SetLocation
	Player->SetActorLocation(ActorLocation);
}

void UCPL_PakrouWallClimbRun::EndActionState()
{
	Super::EndActionState();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector ActorLocation = Player->GetActorLocation();

	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max ���ϱ�
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Z;

		// 3. StaticMesh ���ϱ�.
		// @�����̶��? -> Montage �ٽ� ����
		FVector StaticMeshLocation = ClimbActor->GetStaticMeshComponent()->GetComponentLocation();
		if (ActorLocation.Z < (StaticMeshLocation.Z + (Max.Z * MeshScale) - 95.0f))
		{
			// @SetCurrentBaseAction
			Player->SetCurrentBaseAction(this);

			// @Montage Section - Loop
			Player->ActorStopCurrentAnimMon();
			Player->ActorAnimMonPlay(WallClimbRunMontage, 1.0f, true);
		}
	}
}

