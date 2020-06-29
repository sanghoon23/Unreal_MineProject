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

	// @몽타주 실행
	Player->ActorAnimMonPlay(JumpOverMontage, 1.2f, false);
}

void UCPL_PakrouJumpOver::BeginActionState()
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_JumpOver* JumpOverActor = Cast<ACInteractActor_JumpOver>(PakrouObject);
	if (JumpOverActor != nullptr)
	{
		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1.1 Mesh Min Max 구하기
		JumpOverActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 1.2 Mesh Scale
		MeshScale = JumpOverActor->GetStaticMeshComponent()->GetComponentScale().Y;
		
		// 1.3 Half Height
		float HalfHeight = Player->GetDefaultHalfHeight();

		// 2. 높이 맞추기
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
// JumpOverObject 의 GetIsMeshCollisioning 사용 -
// 캐릭터와 Mesh 가 충돌되는 동안에만 Charactor Forward Move 설정
void UCPL_PakrouJumpOver::TickActionState()
{
	Super::TickActionState();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ACInteractActor_JumpOver* JumpOverActor = Cast<ACInteractActor_JumpOver>(PakrouObject);
	if (JumpOverActor != nullptr)
	{
		// @계속 충돌 중이 아니면 리턴, Mesh 를 벗어낫기 때문에
		IfFalseRet(JumpOverActor->GetIsMeshCollisioning());

		FVector Min, Max;
		float MeshScale = 0.0f;

		// 1.1 Mesh Min Max 구하기
		JumpOverActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// 1.2 Mesh Scale
		MeshScale = JumpOverActor->GetStaticMeshComponent()->GetComponentScale().Y;

		// 2. 앞으로 나아가기. - MeshScale * MeshScale (대략적인 수치 맞추기 위해)
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

	// @중력 켜기
	Player->OnGravity();

	// @이동 가능
	Player->CanMove();

	// @KeyInput Release
	Player->OffBlockKeyInput();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
