#include "CInverseKinematics.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCInverseKinematics::UCInverseKinematics()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UCInverseKinematics::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
	IfNullRet(Character);

	CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// @LineTrace Debug
	bDebugDraw = false;
}


void UCInverseKinematics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector leftNormal, rightNormal;
	float leftTraceDistance = Trace(LeftFootSocket, leftNormal);
	float rightTraceDistance = Trace(RightFootSocket, rightNormal);

	float offset = UKismetMathLibrary::Min(leftTraceDistance, rightTraceDistance);
	offset = (offset > 0.0f) ? 0.0f : offset;

	// Capsule
	float half = CapsuleHalfHeight - FMath::Abs(offset) * 0.5f;
	UCapsuleComponent* capsule = Character->GetCapsuleComponent();
	IfNullRet(capsule);

	float scaleHalf = capsule->GetScaledCapsuleHalfHeight();
	float interpValue = UKismetMathLibrary::FInterpTo(scaleHalf, half, DeltaTime, HipsInterpSpeed);
	capsule->SetCapsuleHalfHeight(interpValue);

	// HipOffset
	Ik.HipOffset = UKismetMathLibrary::FInterpTo(Ik.HipOffset, offset, DeltaTime, HipsInterpSpeed);

	// FootOffset
	Ik.Location_Left = UKismetMathLibrary::FInterpTo(Ik.Location_Left, leftTraceDistance - offset, DeltaTime, FeetInterpSpeed);
	Ik.Location_Right = UKismetMathLibrary::FInterpTo(Ik.Location_Right, -(rightTraceDistance - offset), DeltaTime, FeetInterpSpeed);

	// Rotator
	Ik.Rotation_Left = UKismetMathLibrary::RInterpTo(Ik.Rotation_Left, NormalToRotator(leftNormal), DeltaTime, FeetInterpSpeed);
	Ik.Rotation_Right = UKismetMathLibrary::RInterpTo(Ik.Rotation_Right, NormalToRotator(rightNormal), DeltaTime, FeetInterpSpeed);

}

float UCInverseKinematics::Trace(FName Socket, FVector & OutImpactNormal)
{
	FVector location = Character->GetMesh()->GetSocketLocation(Socket);
	FVector start = FVector(location.X, location.Y, Character->GetActorLocation().Z);

	float z = start.Z - CapsuleHalfHeight - TraceDistance;
	FVector end = FVector(location.X, location.Y, z);

	TArray<AActor *> ignore;
	ignore.Add(GetOwner());

	FHitResult hit;
	EDrawDebugTrace::Type debug = bDebugDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, ignore, debug, hit, true
	);

	OutImpactNormal = hit.ImpactNormal;
	if (hit.IsValidBlockingHit())
	{
		float length = (hit.ImpactPoint - hit.TraceEnd).Size();

		return AdjustOffset + length - TraceDistance;
	}

	return 0.0f;
}

FRotator UCInverseKinematics::NormalToRotator(FVector Normal)
{
	float t = UKismetMathLibrary::DegAtan2(Normal.Y, Normal.Z);
	float t2 = -UKismetMathLibrary::DegAtan2(Normal.X, Normal.Z);

	return FRotator(t2, 0.0f, t);
}

