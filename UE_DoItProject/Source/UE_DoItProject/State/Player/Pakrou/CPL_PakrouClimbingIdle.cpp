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

	// @몽타주 실행
	Player->ActorAnimMonPlay(ClimbingIdleMontage, 1.2f, true);
}

// @Warning -
// 중력 끈 다음 바로 속도를 줄여주어야 됨
// 또한 Action 이 들어왔을 시 OnBlockKeyInput,
// Bind된 값이 아닌,  KeyDown 에 따라서 행동을 관장
void UCPL_PakrouClimbingIdle::BeginActionState()
{
	Super::BeginActionState();

	// @중력 끄기
	Player->OffGravity();

	// @속력 줄이기 - 중력끄고 바로 해줘야함
	Player->GetCharacterMovement()->Velocity = FVector(0.0f);

	// @이동 불가
	Player->CanNotMove();

	// @Warning - KeyInput 불가
	Player->OnBlockKeyInput();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max 구하기
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Z;

		// 3. 거리 맞추기
		SetTargetDistance();

		// 4. 높이 맞추기
		FVector StaticMeshLocation = ClimbActor->GetStaticMeshComponent()->GetComponentLocation();
		FVector ActorLocation = Player->GetActorLocation();
		ActorLocation.Z = (StaticMeshLocation.Z + (Max.Z * MeshScale) - 90.0f);

		// 5. Setting
		Player->SetActorLocation(ActorLocation);
	}
	else
		UE_LOG(LogTemp, Warning, L"PakrouClimbingIdle 'Begin' - ClimbActor NULL");
}

/* Pakrou 연속된 동작 Check Tick */
// @Warning -
// ClimbingIdle 은 Climbing 의 기본 시작 루트.
// 따라서, KeyDown(S) 가 눌려졌을 땐, 반드시 OffBlockKeyInput 을 해주어야함.
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
	// @Mesh 면적까지만 적용되게 하기

	ACInteractActor_Climb* ClimbActor = Cast<ACInteractActor_Climb>(PakrouObject);
	if (ClimbActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1. Mesh Min Max 구하기
		ClimbActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 2. Mesh Scale
		MeshScale = ClimbActor->GetStaticMeshComponent()->GetComponentScale().Z;
	}
}

void UCPL_PakrouClimbingIdle::EndActionState()
{
	Super::EndActionState();

	// @중력 켜기
	Player->OnGravity();

	// @이동 가능
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();
}

