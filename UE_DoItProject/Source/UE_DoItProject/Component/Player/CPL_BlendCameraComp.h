#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPL_BlendCameraComp.generated.h"

UENUM()
enum class EBlendCameraPositionType : uint8
{
	RightSideFace	= 0,
	LeftSideFace	= 1,
	ForwardFace		= 2,
	BottomFace		= 3,
	BackFace		= 4,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_DOITPROJECT_API UCPL_BlendCameraComp : public UActorComponent
{
	GENERATED_BODY()

	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "BlendCamera")
		TArray<class ACPL_BlendCameraActor*> BlendCameras;

	#pragma endregion

public:	
	UCPL_BlendCameraComp();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Function */
public:
	AActor* GetBlendCamera(EBlendCameraPositionType Type);
	void AllResetBlendCameraDefaultValue();

	#pragma	region Member
private:
	class ACPlayer* Player;

	//@RightFace Blend Camera
	FVector		Pos_RightFaceBlendCamera;
	FRotator	Rot_RightFaceBlendCamera;

	//@LeftFace Blend Camera
	FVector		Pos_LeftFaceBlendCamera;
	FRotator	Rot_LeftFaceBlendCamera;

	//@ForwardFace Blend Camera
	FVector		Pos_ForwardFaceBlendCamera;
	FRotator	Rot_ForwardFaceBlendCamera;

	//@BottomFace Blend Camera
	FVector		Pos_BottomFaceBlendCamera;
	FRotator	Rot_BottomFaceBlendCamera;

	//@BackFace Blend Camera
	FVector		Pos_BackFaceBlendCamera;
	FRotator	Rot_BackFaceBlendCamera;

	#pragma endregion
};
