#include "CPL_PakrouBaseAction.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"
#include "Charactor/Player/CPlayer.h"
#include "Actor/Base/CBaseInteractActor.h"

UCPL_PakrouBaseAction::UCPL_PakrouBaseAction()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCPL_PakrouBaseAction::BeginPlay()
{
	Super::BeginPlay();
}

void UCPL_PakrouBaseAction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* Ÿ�ٰ��� �Ÿ��� ���ؼ�, Climb �� �ϱ� ���� ������ ���߾��ֱ� ���� �Լ� */
// @param Target - ��� ( ���⼱ Player )
// ActionInteractSystem �� PlayerHorizontalAtTarget �� ���� ���.
// Player �� ����(GetSimpleCollisionRadius) ���. 
void UCPL_PakrouBaseAction::SetTargetDistance()
{
	ACBaseInteractActor* PakrouActor = Cast<ACBaseInteractActor>(PakrouObject);
	FVector Min, Max;
	FVector MeshScaleVec;
	if (PakrouActor != nullptr)
	{
		// Mesh Min Max ���ϱ�
		PakrouActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// Mesh Scale
		MeshScaleVec = PakrouActor->GetStaticMeshComponent()->GetComponentScale();
	}

	// Location
	FVector ClimbObjectLocation = PakrouActor->GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();

	// Radius
	float CollisionRadius = Player->GetSimpleCollisionRadius() + 10.0f;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector LookDir = ClimbObjectLocation - PlayerLocation;
	LookDir.Z = 0.0f;
	LookDir.Normalize();

	// @1. ���� ���ؼ� ��� ���⿡ �ִ��� Ȯ��
	FVector CrossVec = FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	CrossVec.Normalize();

	// @2. �����ؼ� ���� ���ϱ�
	float RidAngle = FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	RidAngle = acosf(RidAngle);
	float DegAngle = FMath::RadiansToDegrees(RidAngle);

	// @1.1 �ݽð� �����̹Ƿ�, �����ʿ� �����Ѵٸ�
	if (CrossVec.Z >= 0)
	{
		if (DegAngle >= 0.0f && DegAngle < 45.f)
		{
			// @MinX
			PlayerLocation.X
				= ClimbObjectLocation.X + (MeshScaleVec.X * Min.X) - CollisionRadius;

			// @Clamp - Y
			PlayerLocation.Y = FMath::Clamp
			(
				PlayerLocation.Y,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Min.Y) + CollisionRadius,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y) - CollisionRadius
			);
		}
		else if (DegAngle >= 45.f && DegAngle < 135.f)
		{
			// @MinY
			PlayerLocation.Y
				= ClimbObjectLocation.Y + (MeshScaleVec.Y  * Min.Y) - CollisionRadius;

			// @Clamp - X
			PlayerLocation.X = FMath::Clamp
			(
				PlayerLocation.X, 
				ClimbObjectLocation.X + (MeshScaleVec.X * Min.X) + CollisionRadius,
				ClimbObjectLocation.X + (MeshScaleVec.X * Max.X) - CollisionRadius
			);
		}
		else
		{
			// @MaxX
			PlayerLocation.X
				= ClimbObjectLocation.X + (MeshScaleVec.X * Max.X) + CollisionRadius;

			// @Clamp - Y
			PlayerLocation.Y = FMath::Clamp
			(
				PlayerLocation.Y,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Min.Y) + CollisionRadius,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y) - CollisionRadius
			);
		}
	}
	// @1.2 �ݽð� �����̹Ƿ�, ���ʿ� �����Ѵٸ�
	else
	{
		if (DegAngle >= 0.0f && DegAngle < 45.f)
		{
			// @MinX
			PlayerLocation.X
				= ClimbObjectLocation.X + (MeshScaleVec.X * Min.X) - CollisionRadius;

			// @Clamp - Y
			PlayerLocation.Y = FMath::Clamp
			(
				PlayerLocation.Y,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Min.Y) + CollisionRadius,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y) - CollisionRadius
			);
		}
		else if (DegAngle >= 45.f && DegAngle < 135.f)
		{
			// @MaxY
			PlayerLocation.Y
				= ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y) + CollisionRadius;

			// @Clamp - X
			PlayerLocation.X = FMath::Clamp
			(
				PlayerLocation.X,
				ClimbObjectLocation.X + (MeshScaleVec.X * Min.X) + CollisionRadius,
				ClimbObjectLocation.X + (MeshScaleVec.X * Max.X) - CollisionRadius
			);
		}
		else
		{
			// @MaxX
			PlayerLocation.X
				= ClimbObjectLocation.X + (MeshScaleVec.X * Max.X) + CollisionRadius;

			// @Clamp - Y
			PlayerLocation.Y = FMath::Clamp
			(
				PlayerLocation.Y,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Min.Y) + CollisionRadius,
				ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y) - CollisionRadius
			);
		}
	}

	// @SetLocation
	Player->SetActorLocation(PlayerLocation);
}

/* Player �� �ش� PakrouObject �� ������ ������� üũ�ϴ� �Լ� */
// @param Location - Ȯ���� Location
// @������ �����,			return false
// @�̵� ������ �������,		return true
bool UCPL_PakrouBaseAction::CheckClimbingPossible(FVector CheckLocation)
{
	ACBaseInteractActor* PakrouActor = Cast<ACBaseInteractActor>(PakrouObject);
	FVector Min, Max;
	FVector MeshScaleVec;
	if (PakrouActor != nullptr)
	{
		// Mesh Min Max ���ϱ�
		PakrouActor->GetStaticMeshComponent()->GetLocalBounds(Min, Max);

		// Mesh Scale
		MeshScaleVec = PakrouActor->GetStaticMeshComponent()->GetComponentScale();
	}

	// Location
	FVector ClimbObjectLocation = PakrouActor->GetActorLocation();
	FVector MovedLocation = CheckLocation;

	// Radius
	float CollisionRadius = Player->GetSimpleCollisionRadius() + 10.0f;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FVector LookDir = ClimbObjectLocation - MovedLocation;
	LookDir.Z = 0.0f;
	LookDir.Normalize();

	// @1.
	FVector CrossVec = FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	CrossVec.Normalize();

	// @2.
	float RidAngle = FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
	RidAngle = acosf(RidAngle);
	float DegAngle = FMath::RadiansToDegrees(RidAngle);

	// @1.1
	if (CrossVec.Z >= 0)
	{
		if (DegAngle >= 45.f && DegAngle < 135.f)
		{
			CLog::Print(CollisionRadius);
			float MaxPosX = ClimbObjectLocation.X + (MeshScaleVec.X * Max.X);
			float MinPosX = ClimbObjectLocation.X + (MeshScaleVec.X * Min.X);
			if (MovedLocation.X + (CollisionRadius) >= MaxPosX
				|| MovedLocation.X - (CollisionRadius) <= MinPosX)
			{
				return false;
			}
		}
		else
		{
			float MaxPosY = ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y);
			float MinPosY = ClimbObjectLocation.Y + (MeshScaleVec.Y * Min.Y);
			if (MovedLocation.Y + (CollisionRadius) >= MaxPosY
				|| MovedLocation.Y - (CollisionRadius) <= MinPosY)
			{
				return false;
			}
		}
	}
	// @1.2
	else
	{
		if (DegAngle >= 45.f && DegAngle < 135.f)
		{
			float MaxPosX = ClimbObjectLocation.X + (MeshScaleVec.X * Max.X);
			float MinPosX = ClimbObjectLocation.X + (MeshScaleVec.X * Min.X);
			if (MovedLocation.X + (CollisionRadius) >= MaxPosX
				|| MovedLocation.X - (CollisionRadius) <= MinPosX)
			{
				return false;
			}
		}
		else
		{
			float MaxPosY = ClimbObjectLocation.Y + (MeshScaleVec.Y * Max.Y);
			float MinPosY = ClimbObjectLocation.Y + (MeshScaleVec.Y * Min.Y);
			if (MovedLocation.Y + (CollisionRadius) >= MaxPosY
				|| MovedLocation.Y - (CollisionRadius) <= MinPosY)
			{
				return false;
			}
		}
	}

	// @�����ϴٸ�
	return true;
}
