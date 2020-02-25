#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInverseKinematics.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FInverseKinematics
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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCInverseKinematics : public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		FName LeftFootSocket = "Foot_L";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		FName RightFootSocket = "Foot_R";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float TraceDistance = 55.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float AdjustOffset = 0.1f; // 발과 땅 사이의 간격.

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float HipsInterpSpeed = 17.0f; // (Pelvis)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
		float FeetInterpSpeed = 22.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InverseKinematics")
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
private:
	float Trace(FName Socket, FVector& OutImpactNormal);
	FRotator NormalToRotator(FVector Normal);

	#pragma	region Member
private:
	class ACharacter* Character;

	float CapsuleHalfHeight;
	FInverseKinematics Ik;

public:
	FInverseKinematics& GetEffector() { return Ik; }
	#pragma endregion
};
