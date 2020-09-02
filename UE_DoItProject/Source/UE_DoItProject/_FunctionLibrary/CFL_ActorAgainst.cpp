#include "CFL_ActorAgainst.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Interface/IC_Charactor.h"

UCFL_ActorAgainst::UCFL_ActorAgainst()
{
}

void UCFL_ActorAgainst::LookAtTarget(AActor * Subject, const AActor* Target)
{
	check(Target);
	check(Subject);

	/* 
	Montage 가 실행될 때, 타겟을 바라본다는 것을 가정한 상태에서
	주체는 Charactor->DontMontagePlay 가 되었다면 바라보지 않음
	Ex) - 공격 상황,
	*/
	IIC_Charactor* I_Charactor = Cast<IIC_Charactor>(Subject);
	if (I_Charactor != nullptr)
	{
		bool bDontMontagePlay = I_Charactor->IsDontMontagePlay();
		if (bDontMontagePlay == true)
		{
			return;
		}
	}

	//@Look At Target
	FVector DestVec = Target->GetActorLocation() - Subject->GetActorLocation();
	DestVec.Z = 0.0f;
	DestVec.Normalize();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Subject->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}

void UCFL_ActorAgainst::LookAtPoint(AActor * Subject, FVector& Point)
{
	check(Subject);

	FVector DestVec = Point - Subject->GetActorLocation();
	DestVec.Z = 0.0f;
	DestVec.Normalize();
	FRotator Rotator = FRotationMatrix::MakeFromX(DestVec).Rotator();
	Subject->SetActorRotation(FRotator(0.0f, Rotator.Yaw, 0.0f));
}

void UCFL_ActorAgainst::ActorLocateFrontTarget(const AActor * Target, AActor* Subject, float Distance, bool HeightInclude)
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

bool UCFL_ActorAgainst::IsTargetInAir(const ACharacter * Target)
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
