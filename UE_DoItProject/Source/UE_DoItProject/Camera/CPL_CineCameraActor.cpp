#include "CPL_CineCameraActor.h"
#include "Global.h"
#include "Engine/EngineTypes.h"
#include "Kismet/KismetMathLibrary.h"

void ACPL_CineCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACPL_CineCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStart == true)
	{
		if (Target == nullptr)
		{
			bStart = false;
			return;
		}

		FVector ForwardVec = GetActorForwardVector();
		FVector Location = GetActorLocation();
		FVector TargetLocation = Target->GetActorLocation();
		FVector Dir = TargetLocation - Location;
		Dir.Normalize();
		FRotator Rotation = GetActorRotation();
		FRotator TargetRotation = Target->GetActorRotation();

		if (TargetLocation.Equals(Location) && TargetRotation.Equals(Rotation))
		{
			bStart = false;
			return;
		}

		//@Set Rotation
		if (TargetRotation.Equals(Rotation) == false)
		{
			FRotator Rot = UKismetMathLibrary::RInterpTo(Rotation, TargetRotation, DeltaTime, RotationInterpSpeed);
			SetActorRotation(Rot);
		}

		//@Set Location
		if (TargetLocation.Equals(Location, 3.0f) == false)
		{
			Location += Dir * LocationSpeed * DeltaTime;
			SetActorLocation(Location);
		}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//FRotator Rot = UKismetMathLibrary::MakeRotFromX(Dir);
		//FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Location, TargetLocation);

		//CLog::Print(FVector(RetRot));
		//CLog::Print(Location);

		//FRotator Rot = UKismetMathLibrary::MakeRotFromX(Dir);
		//UKismetMathLibrary::Lerp(Rot)
		//FRotator Rot = UKismetMathLibrary::make(Location, LookAt);
	}
}

void ACPL_CineCameraActor::MoveToTarget()
{
	check(Target);

	//@Detach
	EDetachmentRule Rules = EDetachmentRule::KeepWorld;
	FDetachmentTransformRules TransformRules(Rules, false);
	DetachFromActor(TransformRules);

	//@Set Speed
	float Dist = GetDistanceTo(Target);
	LocationSpeed = (Dist / TimeArrivalToTaget);

	//@Start!!
	bStart = true;
}
