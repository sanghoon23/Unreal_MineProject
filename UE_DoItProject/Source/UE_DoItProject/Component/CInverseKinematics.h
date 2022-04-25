#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInverseKinematics.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FFeetInverseKinematics
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Location_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Location_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float HipOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator Rotation_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator Rotation_Right;
};

USTRUCT(Atomic, BlueprintType)
struct FHandsInverseKinematics
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector Location_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector Location_Right;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float HandAlpha_Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float HandAlpha_Right;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCInverseKinematics 
	: public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	/* @Feet */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		FName LeftFootSocket = "Foot_L";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		FName RightFootSocket = "Foot_R";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float HipsInterpSpeed = 17.0f; // (Pelvis)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float FeetTraceDistance = 55.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float FeetAdjustOffset = 0.1f; // 발과 땅 사이의 간격.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float FeetInterpSpeed = 1.0f;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/* @Hand */

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		FName LeftHandSocket = "Hand_L";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		FName RightHandSocket = "hand_r";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		FName LeftLowerArmSocket = "lowerarm_l";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		FName RightLowerArmSocket = "lowerarm_r";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		float LineTraceDistance = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		float HeightOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		float ShpereRadius = 7.0f;

	////UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
	////	float IKLeftHandAlpha = 7.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
	//	float HandTraceDistance = 5.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
	//	float HandAdjustOffset = 10.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
	//	float HandInterpSpeed = 30.0f;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HandInverseKinematics")
		bool bDebugDraw = false;

private:

	#pragma endregion

public:	
	UCInverseKinematics();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/* Function */
public:
	void OnLeftHandIK() { bRunningLeftHandIK = true; }
	void OffLeftHandIK() { bRunningLeftHandIK = false; }

	void OnRightHandIK() { bRunningRightHandIK = true; }
	void OffRightHandIK() { bRunningRightHandIK = false; }

private:
	float FeetTrace(FName Socket, FVector& OutImpactNormal);
	void LeftHandTrace();
	void RightHandTrace();

	FRotator NormalToRotator(FVector Normal);

	#pragma	region Member
public:
	FFeetInverseKinematics&		GetFeetIKEffector() { return FeetIk; }
	FHandsInverseKinematics&	GetHandIKEffector() { return HandIK; }

private:
	class ACharacter*		Character;

	// Foot IK
	float					CapsuleHalfHeight;
	FFeetInverseKinematics	FeetIk;

	// Hand IK
	bool bRunningLeftHandIK		= false;
	bool bRunningRightHandIK	= false;

	FHandsInverseKinematics		HandIK;

	#pragma endregion
};
