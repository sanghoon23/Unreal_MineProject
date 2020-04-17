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

/* InteractActor Type �� 'JUMPOVER' �� �� ����Ǵ� �Լ� */
void UCPL_ActionInteractSystem::JumpOverInterFunc(FHitResult& HitResult)
{
	// @Horizontal Rotation
	PlayerHorizontalAtTarget(HitResult.GetActor());

	// @StartAction
	PakrouActionComp->StartAction(PakrouStartActionType::JUMPOVER, HitResult.GetActor());
}

/* InteractActor Type �� 'CLIMB' �� �� ����Ǵ� �Լ� */
void UCPL_ActionInteractSystem::ClimbInterFunc(FHitResult& HitResult)
{
	// @���(InteractActor) �� Player Vector
	FVector LookDir = HitResult.GetActor()->GetActorLocation() - Player->GetActorLocation();
	LookDir.Normalize();

	// @UpVector �� ���� ���ϱ�
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

/* ������κ��� �����ϰ� ȸ�� (4��) */
// Ex) ���� Cube ��� Player �� ȸ���� ��� 
// �� ��ġ�� ���� ���� ������ �����ϰ� ȸ����Ű�� �Լ�
// @param Target - ���
void UCPL_ActionInteractSystem::PlayerHorizontalAtTarget(AActor * Target)
{
	// @1. ���(InteractActor) �� Player Vector ���ϱ�
	FVector LookDir = Target->GetActorLocation() - Player->GetActorLocation();
	LookDir.Z = 0.0f;
	LookDir.Normalize();

	// @2. ���� ���ؼ� ��� ���⿡ �ִ��� Ȯ��
	FVector CrossVec = FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	CrossVec.Normalize();

	// @3. �����ؼ� ���� ���ϱ�
	float RidAngle = FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	RidAngle = acosf(RidAngle);
	float DegAngle = FMath::RadiansToDegrees(RidAngle);

	// @2.1 �ݽð� �����̹Ƿ�, ���(InteractActor) �� �����ʿ� �����Ѵٸ�
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
	// @2.2 �ݽð� �����̹Ƿ�, ���(InteractActor) �� ���ʿ� �����Ѵٸ�
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


