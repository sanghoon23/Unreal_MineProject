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

	// @Character
	Character = Cast<ACharacter>(GetOwner());
	IfNullRet(Character);

	// @CapsuleHeight
	CapsuleHalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// @LineTrace Debug
	bDebugDraw = true;
}


// @For Feets
// 1. Kinematic 을 위한 Left, Right Foot 회전 값을 계산 - FFeetInverseKinematics
void UCInverseKinematics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Feet IK Calc
	{
		FVector leftNormal, rightNormal;
		float leftTraceDistance = FeetTrace(LeftFootSocket, leftNormal);
		float rightTraceDistance = FeetTrace(RightFootSocket, rightNormal);

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
		FeetIk.HipOffset = UKismetMathLibrary::FInterpTo(FeetIk.HipOffset, offset, DeltaTime, HipsInterpSpeed);

		// FootOffset
		FeetIk.Location_Left = UKismetMathLibrary::FInterpTo(FeetIk.Location_Left, leftTraceDistance - offset, DeltaTime, FeetInterpSpeed);
		FeetIk.Location_Right = UKismetMathLibrary::FInterpTo(FeetIk.Location_Right, -(rightTraceDistance - offset), DeltaTime, FeetInterpSpeed);

		// Rotator
		FeetIk.Rotation_Left = UKismetMathLibrary::RInterpTo(FeetIk.Rotation_Left, NormalToRotator(leftNormal), DeltaTime, FeetInterpSpeed);
		FeetIk.Rotation_Right = UKismetMathLibrary::RInterpTo(FeetIk.Rotation_Right, NormalToRotator(rightNormal), DeltaTime, FeetInterpSpeed);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @LeftHandIK
	if (bRunningLeftHandIK == true)
	{
		LeftHandTrace();
	}
	else
	{
		HandIK.HandAlpha_Left = 0.0f;
		HandIK.Location_Left = FVector(0.0f);

		//HandIK.HandAlpha_Left = UKismetMathLibrary::Lerp(HandIK.HandAlpha_Left, 0.0f, 0.2f);
		//HandIK.Location_Left = UKismetMathLibrary::VLerp(HandIK.Location_Left, FVector(0.0f), 1.0f);
	}

	// @RightHandIK
	if (bRunningRightHandIK == true)
	{
		RightHandTrace();
	}
	else
	{
		HandIK.HandAlpha_Right = 0.0f;
		HandIK.Location_Right = FVector(0.0f);

		//HandIK.HandAlpha_Right = UKismetMathLibrary::Lerp(HandIK.HandAlpha_Right, 0.0f, 0.2f);
		//HandIK.Location_Right = UKismetMathLibrary::VLerp(HandIK.Location_Right, FVector(0.0f), 1.0f);
	}

}

/* Feet Line Trace For FeetSocket */
float UCInverseKinematics::FeetTrace(FName Socket, FVector & OutImpactNormal)
{
	FVector location = Character->GetMesh()->GetSocketLocation(Socket);
	FVector start = FVector(location.X, location.Y, Character->GetActorLocation().Z);

	float z = start.Z - CapsuleHalfHeight - FeetTraceDistance;
	FVector end = FVector(location.X, location.Y, z);

	TArray<AActor *> ignore;
	ignore.Add(GetOwner());

	FHitResult hit;
	EDrawDebugTrace::Type debug = bDebugDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, ignore, debug, hit, true
	);

	OutImpactNormal = hit.ImpactNormal;
	if (hit.IsValidBlockingHit())
	{
		float length = (hit.ImpactPoint - hit.TraceEnd).Size();

		return FeetAdjustOffset + length - FeetTraceDistance;
	}

	return 0.0f;
}

/* IK 를 위한 Left Hand Line Trace Hitter(왼손 IK) */
// @Warning - OnHandIK, OffHandIK 호출 동작(bRunningHandIK)
// RightVector 로 약간의 거리조정,
void UCInverseKinematics::LeftHandTrace()
{
	FVector ForwardVec = Character->GetActorForwardVector();
	FVector SocketLocation = Character->GetMesh()->GetSocketLocation(LeftHandSocket);

	float CharactorHeight = Character->GetActorLocation().Z + Character->GetDefaultHalfHeight() + HeightOffset;

	FVector Start	= FVector(SocketLocation.X, SocketLocation.Y, CharactorHeight);
	FVector End = Start + (ForwardVec * LineTraceDistance);

	TArray<AActor *> Ignore;
	Ignore.Add(GetOwner());

	FHitResult HitResult;
	EDrawDebugTrace::Type Debug = bDebugDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, Ignore, Debug, HitResult, true
	);

	if (HitResult.IsValidBlockingHit())
	{
		HandIK.HandAlpha_Left	= 0.5f;

		FVector RightVec = Character->GetActorRightVector();
		HandIK.Location_Left = HitResult.Location - (RightVec * 10.0f);

		// Test Code
		//FString PrintStr = L"Left Location : " + HandIK.Location_Left.ToString();
		//CLog::Print(PrintStr);
	}
}

/* IK 를 위한 Right Hand Line Trace Hitter(오른손 IK) */
// @Warning - OnHandIK, OffHandIK 호출 동작(bRunningHandIK)
// RightVector 로 약간의 거리조정,
void UCInverseKinematics::RightHandTrace()
{
	FVector ForwardVec = Character->GetActorForwardVector();
	float CharactorHeight = Character->GetActorLocation().Z + Character->GetDefaultHalfHeight() + HeightOffset;
	FVector SocketLocation = Character->GetMesh()->GetSocketLocation(RightHandSocket);

	FVector Start = FVector(SocketLocation.X, SocketLocation.Y, CharactorHeight);
	FVector End = Start + (ForwardVec * LineTraceDistance);

	TArray<AActor *> Ignore;
	Ignore.Add(GetOwner());

	FHitResult HitResult;
	EDrawDebugTrace::Type Debug = bDebugDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, Ignore, Debug, HitResult, true
	);

	if (HitResult.IsValidBlockingHit())
	{
		HandIK.HandAlpha_Right = 0.5f;

		FVector RightVec = Character->GetActorRightVector();
		HandIK.Location_Right = HitResult.Location + (RightVec * 10.0f);

		// Test Code
		//FString PrintStr = L"Right Location : " + HandIK.Location_Right.ToString();
		//CLog::Print(PrintStr);
	}
}

FRotator UCInverseKinematics::NormalToRotator(FVector Normal)
{
	float t = UKismetMathLibrary::DegAtan2(Normal.Y, Normal.Z);
	float t2 = -UKismetMathLibrary::DegAtan2(Normal.X, Normal.Z);

	return FRotator(t2, 0.0f, t);
}

