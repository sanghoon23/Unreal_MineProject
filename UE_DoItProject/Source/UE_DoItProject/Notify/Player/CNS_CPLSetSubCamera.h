#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "Component/Player/CPL_BlendCameraComp.h"

#include "CNS_CPLSetSubCamera.generated.h"

UCLASS()
class UE_DOITPROJECT_API UCNS_CPLSetSubCamera
	: public UAnimNotifyState
{
	GENERATED_BODY()

#pragma region Reflection
public:
	UPROPERTY(EditAnywhere, Category = "Value")
		bool bUsingTransform = false;

	UPROPERTY(EditAnywhere, Category = "Value")
		FVector LocationOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
		FRotator RotationOffset;

	UPROPERTY(EditAnywhere, Category = "Value")
		EBlendCameraPositionType BlendPosType;

	UPROPERTY(EditAnywhere, Category = "Value")
		float StartBlendTime = 0.0f; //@Default

	UPROPERTY(EditAnywhere, Category = "Value")
		bool bUsingEndBlendOrigin = false;

	UPROPERTY(EditAnywhere, Category = "Value")
		float EndBlendTime = 0.0f; //@Default

	UPROPERTY(EditAnywhere, Category = "Value")
		bool bUsingGlobalTimer = false;

	UPROPERTY(EditAnywhere, Category = "Value")
		float GlobalTimeAttack = 0.0f;

#pragma endregion

private:
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	
	/* Member */
private:

};
