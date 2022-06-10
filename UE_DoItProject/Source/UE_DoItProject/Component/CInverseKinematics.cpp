#include "CInverseKinematics.h"
#include "Global.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
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
}


// @For Feets
void UCInverseKinematics::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// @Feet IK Calc
	{
		FVector LeftNormal, RightNormal;
		float LeftTraceDistance = FeetTrace(LeftFootSocket, LeftNormal);
		float RightTraceDistance = FeetTrace(RightFootSocket, RightNormal);

		float Offset = UKismetMathLibrary::Min(LeftTraceDistance, RightTraceDistance);
		Offset = (Offset > 0.0f) ? 0.0f : Offset;

		// Capsule
		float Half = CapsuleHalfHeight - FMath::Abs(Offset) * 0.5f;
		UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
		IfNullRet(Capsule);

		float ScaleHalf = Capsule->GetScaledCapsuleHalfHeight();
		float InterpValue = UKismetMathLibrary::FInterpTo(ScaleHalf, Half, DeltaTime, HipsInterpSpeed);
		Capsule->SetCapsuleHalfHeight(InterpValue);

		// HipOffset
		FeetIk.HipOffset = UKismetMathLibrary::FInterpTo(FeetIk.HipOffset, Offset, DeltaTime, HipsInterpSpeed);

		// FootOffset
		FeetIk.Location_Left = UKismetMathLibrary::FInterpTo(FeetIk.Location_Left, LeftTraceDistance, DeltaTime, FeetInterpSpeed);
		FeetIk.Location_Right = UKismetMathLibrary::FInterpTo(FeetIk.Location_Right, -(RightTraceDistance), DeltaTime, FeetInterpSpeed);

		// Rotator
		FeetIk.Rotation_Left = UKismetMathLibrary::RInterpTo(FeetIk.Rotation_Left, NormalToRotator(LeftNormal), DeltaTime, FeetInterpSpeed);
		FeetIk.Rotation_Right = UKismetMathLibrary::RInterpTo(FeetIk.Rotation_Right, NormalToRotator(RightNormal), DeltaTime, FeetInterpSpeed);
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

float UCInverseKinematics::FeetTrace(FName Socket, FVector & OutImpactNormal)
{
	float Scale = Character->GetTransform().GetScale3D().Z;
	FVector ActorLocation = Character->GetActorLocation();

	FVector SocketLocation = Character->GetMesh()->GetSocketLocation(Socket);
	FVector Start = FVector(SocketLocation.X, SocketLocation.Y, ActorLocation.Z);

	float Z = ActorLocation.Z - CapsuleHalfHeight - FeetTraceDistance;
	FVector End = FVector(SocketLocation.X, SocketLocation.Y, Z);


	TArray<AActor *> Ignore;
	Ignore.Add(GetOwner());

	FHitResult Hit;
	EDrawDebugTrace::Type Debug
		= (bDebugDraw == true) ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel9),
		true, Ignore, Debug, Hit, true
	);

	OutImpactNormal = Hit.ImpactNormal;
	if (Hit.IsValidBlockingHit())
	{
		float Length = (End - Hit.Location).Size() / Scale;

		return Length - FeetTraceDistance + FeetAdjustOffset;
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
	FVector End		= Start + (ForwardVec * LineTraceDistance);

	TArray<AActor *> Ignore;
	Ignore.Add(GetOwner());

	FHitResult HitResult;
	EDrawDebugTrace::Type Debug = bDebugDraw ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false, Ignore, Debug, HitResult, true
	);

	if (HitResult.IsValidBlockingHit())
	{
		HandIK.HandAlpha_Left	= 0.5f;

		FVector RightVec = Character->GetActorRightVector();
		HandIK.Location_Left = HitResult.Location - (RightVec * 10.0f);
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
		GetWorld(), Start, End, UEngineTypes::ConvertToTraceType(ECC_Pawn),
		true, Ignore, Debug, HitResult, true
	);

	if (HitResult.IsValidBlockingHit())
	{
		HandIK.HandAlpha_Right = 0.5f;

		FVector RightVec = Character->GetActorRightVector();
		HandIK.Location_Right = HitResult.Location + (RightVec * 10.0f);
	}
}

FRotator UCInverseKinematics::NormalToRotator(FVector Normal)
{
	float t = UKismetMathLibrary::DegAtan2(Normal.Y, Normal.Z);
	float t2 = -UKismetMathLibrary::DegAtan2(Normal.X, Normal.Z);

	return FRotator(t2, 0.0f, t);
}

