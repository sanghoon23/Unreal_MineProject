#include "CPL_PakrouJumpOver.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/CInteractActor_JumpOver.h"

UCPL_PakrouJumpOver::UCPL_PakrouJumpOver()
{
	PrimaryComponentTick.bCanEverTick = false;

	FString Path = L"";

	// Set JumpOver Montage
	{
		Path = L"AnimMontage'/Game/_Mine/Montages/Player/Common/Pakrou/CommonMon_JumpOver.CommonMon_JumpOver'";
		ConstructorHelpers::FObjectFinder<UAnimMontage> JumpOverAction(*Path);
		if (JumpOverAction.Succeeded())
			JumpOverMontage = JumpOverAction.Object;
	}
}

void UCPL_PakrouJumpOver::BeginPlay()
{
	Super::BeginPlay();

	// @Set PlayerActor
	Player = Cast<ACPlayer>(GetOwner());
	check(Player);
}

void UCPL_PakrouJumpOver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_PakrouJumpOver::OnAction()
{
	Super::OnAction();

	// @Reset
	// Player->OnActionResetState.Broadcast(Player);

////////////////////////////////////////////////////////////////////////////////////////////////

	// @SetCurrentBaseAction
	Player->SetCurrentBaseAction(this);

	// @��Ÿ�� ����
	Player->ActorAnimMonPlay(JumpOverMontage, 1.2f, false);
}

void UCPL_PakrouJumpOver::BeginActionState()
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_JumpOver* JumpOverActor = Cast<ACInteractActor_JumpOver>(PakrouObject);
	if (JumpOverActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1.1 Mesh Min Max ���ϱ�
		JumpOverActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 1.2 Mesh Scale
		MeshScale = JumpOverActor->GetStaticMeshComponent()->GetComponentScale().Y;
		
		// 1.3 Half Height
		float HalfHeight = Player->GetDefaultHalfHeight();

		// 2. ���� ���߱�
		FVector StaticMeshLocation = JumpOverActor->GetStaticMeshComponent()->GetComponentLocation();
		FVector ActorLocation = Player->GetActorLocation();
		ActorLocation.Z = (StaticMeshLocation.Z + (Max.Y * MeshScale) + HalfHeight);

		// 3. Setting
		Player->SetActorLocation(ActorLocation);
	}
	else
		UE_LOG(LogTemp, Warning, L"PakrouJumOver 'Begin' - JumpOverActor NULL");
}

// @Warning - 
// JumpOverObject �� GetIsMeshCollisioning ��� -
// ĳ���Ϳ� Mesh �� �浹�Ǵ� ���ȿ��� Charactor Forward Move ����
void UCPL_PakrouJumpOver::TickActionState()
{
	Super::TickActionState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_JumpOver* JumpOverActor = Cast<ACInteractActor_JumpOver>(PakrouObject);
	if (JumpOverActor != nullptr)
	{
		// @��� �浹 ���� �ƴϸ� ����, Mesh �� ����� ������
		IfFalseRet(JumpOverActor->GetIsMeshCollisioning());

		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1.1 Mesh Min Max ���ϱ�
		JumpOverActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 1.2 Mesh Scale
		MeshScale = JumpOverActor->GetStaticMeshComponent()->GetComponentScale().Y;

		// 2. ������ ���ư���. - MeshScale * MeshScale (�뷫���� ��ġ ���߱� ����)
		float Speed = JumpOverSpeed;
		FVector ActorLocation = Player->GetActorLocation();
		FVector ActorForwardDir = Player->GetActorForwardVector();
		ActorLocation += (ActorForwardDir * Speed * (MeshScale * MeshScale));

		// 3. Setting
		Player->SetActorLocation(ActorLocation);
	}
	else
		UE_LOG(LogTemp, Warning, L"PakrouJumOver 'Tick' - JumpOverActor NULL");
}

void UCPL_PakrouJumpOver::EndActionState()
{
	Super::EndActionState();

	// @�߷� �ѱ�
	Player->OnGravity();

	// @�̵� ����
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
