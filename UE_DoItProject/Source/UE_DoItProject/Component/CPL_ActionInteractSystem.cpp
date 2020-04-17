#include "CPL_ActionInteractSystem.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Actor/Base/CBaseInteractActor.h"
#include "Charactor/Player/CPlayer.h"

UCPL_ActionInteractSystem::UCPL_ActionInteractSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	PakrouActionComp = CreateDefaultSubobject<UCPL_PakrouActionComp>("PakrouActionComponent");
}

void UCPL_ActionInteractSystem::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACPlayer>(GetOwner());
}


void UCPL_ActionInteractSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPL_ActionInteractSystem::InteractInput()
{
	// @IF TRUE RETURN
	IfTrueRet(Player->GetEvade()); //@Evade Check
	// IfTrueRet(Player->IsJumping());
	// IfTrueRet(Player->GetCharacterMovement()->IsFalling());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

////////////////////////////////////////////////////////////////////////////////////////////////

	FVector ActorForward = Player->GetActorForwardVector();
	FVector Start = Player->GetActorLocation();
	FVector End = Player->GetActorLocation() + (ActorForward * InteractDistance);

	FCollisionShape sphere = FCollisionShape::MakeSphere(InteractRadius);
	FCollisionQueryParams CollisionQueryParm(NAME_None, false, Player);

	FHitResult HitResult;
	float DebugLifeTime = 1.0f;
	bool bResult = GetWorld()->SweepSingleByChannel
	(
		HitResult
		, Start
		, End
		, FQuat::Identity
		, ECC_GameTraceChannel7 // @Player Action
		, sphere
		, CollisionQueryParm
	);

#if  ENABLE_DRAW_DEBUG

	DrawDebugSphere(GetWorld(), End, sphere.GetSphereRadius(), 40, FColor::Green, false, DebugLifeTime);

#endif // ENABLE_DRAW_DEBUG

	if (bResult == true)
	{
		// @Reset
		Player->OnActionResetState.Broadcast(Player);

		IIC_InteractActor* InteractActor = Cast<IIC_InteractActor>(HitResult.GetActor());
		if (InteractActor != nullptr)
		{
			// @Set InteractActor
			Player->SetCurrentInteractActor(InteractActor);

			// JUMPOVER
			if (InteractActor->GetInteractType() == InteractType::JUMPOVER)
			{
				JumpOverInterFunc(HitResult);
			}
			// CLIMB
			else if (InteractActor->GetInteractType() == InteractType::CLIMB)
			{
				ClimbInterFunc(HitResult);
			}
		}
	}//(bResult == true)
}

/* InteractActor Type 이 'JUMPOVER' 일 때 실행되는 함수 */
void UCPL_ActionInteractSystem::JumpOverInterFunc(FHitResult& HitResult)
{
	// @Horizontal Rotation
	PlayerHorizontalAtTarget(HitResult.GetActor());

	// @StartAction
	PakrouActionComp->StartAction(PakrouStartActionType::JUMPOVER, HitResult.GetActor());
}

/* InteractActor Type 이 'CLIMB' 일 때 실행되는 함수 */
void UCPL_ActionInteractSystem::ClimbInterFunc(FHitResult& HitResult)
{
	// @대상(InteractActor) 와 Player Vector
	FVector LookDir = HitResult.GetActor()->GetActorLocation() - Player->GetActorLocation();
	LookDir.Normalize();

	// @UpVector 와 각도 구하기
	float RidAngle = FVector::DotProduct(FVector(0.0f, 0.0f, 1.0f), LookDir);
	RidAngle = acosf(RidAngle);
	float DegAngle = FMath::RadiansToDegrees(RidAngle);

	if (DegAngle > 90.0f)
	{
		// CLog::Print(L"Upper");
		Player->StopJumping();
		PlayerHorizontalAtTarget(HitResult.GetActor());

		PakrouActionComp->StartAction(PakrouStartActionType::IDLE, HitResult.GetActor());
	}
	else
	{
		// CLog::Print(L"Downer");
		PlayerHorizontalAtTarget(HitResult.GetActor());

		PakrouActionComp->StartAction(PakrouStartActionType::WALLCLIMBRUN, HitResult.GetActor());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* 대상으로부터 수평하게 회전 (4면) */
// Ex) 만약 Cube 라면 Player 의 회전이 어떻든 
// 그 위치에 따라 면의 직선과 동일하게 회전시키는 함수
// @param Target - 대상
void UCPL_ActionInteractSystem::PlayerHorizontalAtTarget(AActor * Target)
{
	// @1. 대상(InteractActor) 와 Player Vector 구하기
	FVector LookDir = Target->GetActorLocation() - Player->GetActorLocation();
	LookDir.Z = 0.0f;
	LookDir.Normalize();

	// @2. 외적 구해서 어느 방향에 있는지 확인
	FVector CrossVec = FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	CrossVec.Normalize();

	// @3. 내적해서 각도 구하기
	float RidAngle = FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	RidAngle = acosf(RidAngle);
	float DegAngle = FMath::RadiansToDegrees(RidAngle);

	// @2.1 반시계 방향이므로, 대상(InteractActor) 이 오른쪽에 존재한다면
	if (CrossVec.Z >= 0)
	{
		if (DegAngle >= 0.0f && DegAngle < 45.f)
		{
			Player->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (DegAngle >= 45.f && DegAngle < 135.f)
		{
			Player->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
		}
		else
		{
			Player->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
	}
	// @2.2 반시계 방향이므로, 대상(InteractActor) 이 왼쪽에 존재한다면
	else
	{
		if (DegAngle >= 0.0f && DegAngle < 45.f)
		{
			Player->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
		else if (DegAngle >= 45.f && DegAngle < 135.f)
		{
			Player->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		}
		else
		{
			Player->SetActorRotation(FRotator(0.0f, -180.0f, 0.0f));
		}
	}
}


