#include "CFL_ActorAgainst.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCFL_ActorAgainst::UCFL_ActorAgainst()
{
}

void UCFL_ActorAgainst::LookAtTarget(AActor * Target, AActor* Subject)
{
	check(Target);
	check(Subject);

	FVector DestVec = Target->GetActorLocation() - Subject->GetActorLocation();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Subject->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}

void UCFL_ActorAgainst::ActorLocateFrontTarget(AActor * Target, AActor* Subject, float Distance, bool HeightInclude)
{
	check(Target);
	check(Subject);
	FVector TargetLocation = Target->GetActorLocation();
	FVector SubjectLocation = Subject->GetActorLocation();
	FVector LookDir = TargetLocation - SubjectLocation;
	LookDir.Normalize();
	LookDir.Z = 0.0f;

	// @Target 에서 Distance 만큼 떨어진 곳으로 위치시키기 위해
	FVector SettingLocation = TargetLocation;

	// @LookDir 을 이용해 , 거리 맞추고
	SettingLocation += (-1) * LookDir * (Distance + 0.0f);

	if (HeightInclude == true) // @Target 의 높이에 맞추고,
	{
		SettingLocation.Z = TargetLocation.Z;
	}

	// @Setting Location
	Subject->SetActorLocation(SettingLocation);
}

void UCFL_ActorAgainst::SetAngleWithControlRot(ACharacter* Subject, float Angle)
{
	check(Subject);
	FRotator ControlRotation = Subject->GetControlRotation();
	FRotator Y = FRotator(0.0f, ControlRotation.Yaw + Angle, 0.0f);
	FVector Right = FQuat(Y).GetRightVector();

	Subject->SetActorRotation(Y);
}

bool UCFL_ActorAgainst::IsTargetInAir(ACharacter * Target)
{
	check(Target);
	if (Target->GetCharacterMovement()->IsFalling()
		&& Target->GetCharacterMovement()->GravityScale <= 0.0f)
	{
		return true;
	}

	if (Target->GetCharacterMovement()->IsFalling())
	{
		return true;
	}

	return false;
}
