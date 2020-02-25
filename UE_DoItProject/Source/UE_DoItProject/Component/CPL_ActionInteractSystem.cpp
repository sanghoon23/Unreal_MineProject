#include "CPL_ActionInteractSystem.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_InteractActor.h"
#include "Actor/Base/CBaseInteractActor.h"
#include "Charactor/Player/CPlayer.h"
#include "State/Player/CPL_ActionJumpOver.h"

UCPL_ActionInteractSystem::UCPL_ActionInteractSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	JumpOver = CreateDefaultSubobject<UCPL_ActionJumpOver>("JumpOver");
	JumpOver->SetOwnerPawn(Cast<APawn>(GetOwner()));
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
	IfTrueRet(Player->IsJumping());
	IfTrueRet(Player->GetCharacterMovement()->IsFalling());
	IfTrueRet(Player->GetIEquipComp()->GetEquiping());

	// @IF FALSE RETURN
	IfFalseRet(Player->GetCanMove());

	// @Reset
	Player->OnActionResetState.Broadcast(Player);

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
		// CLog::Print(HitResult.GetActor()->GetName());

		IIC_InteractActor* InteractActor = Cast<IIC_InteractActor>(HitResult.GetActor());
		if (InteractActor != nullptr)
		{
			// @Set InteractActor
			Player->SetCurrentInteractActor(InteractActor);

			// 1.JUMPOVER
			if (InteractActor->GetInteractType() == InteractType::JUMPOVER)
			{
				// @Set Interactive Data
				FInteractiveData Data;
				Data.JumpOverSpeed = 0.3f;
				InteractActor->SetInteractiveData(Data);

				// @Player 회전
				FVector LookDir = HitResult.GetActor()->GetActorLocation() - Player->GetActorLocation();
				LookDir.Z = 0.0f;
				LookDir.Normalize();

				FVector CrossVec = FVector::CrossProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
				CrossVec.Normalize();
				// CLog::Print(CrossVec.ToString());
				if (CrossVec.Z >= 0)
				{
					float RidAngle = FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
					// CLog::Print(RidAngle);
					RidAngle = acosf(RidAngle);
					// CLog::Print(RidAngle);

					float DegAngle = FMath::RadiansToDegrees(RidAngle);
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
				else
				{
					float RidAngle = FVector::DotProduct(FVector(1.0f, 0.0f, 0.0f), LookDir);
					// CLog::Print(RidAngle);
					RidAngle = acosf(RidAngle);
					// CLog::Print(RidAngle);

					float DegAngle = FMath::RadiansToDegrees(RidAngle);
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


				// CLog::Print(L"InteractActor In OnAction");
				JumpOver->OnAction(); // @실행 시, CNS_Interact Notify
			}
		}

	}
}

