#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Components/TimelineComponent.h"

#include "TestCameraActor.generated.h"

UCLASS()
class UE_DOITPROJECT_API ATestCameraActor 
	: public ACameraActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:
	UPROPERTY(VisibleAnywhere)
		class USplineComponent* Spline;

	UPROPERTY(VisibleAnywhere)
		class UCurveFloat* CurveFloat;

	#pragma endregion

public:
	ATestCameraActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	/* Function */
public:
	void OnPlay();

private:
	void UpdateCameraMove(UCurveFloat * CurveFloat);
	void UpdateActorLocationWithSpline(float fValue);

	/* Member */
private:
	FTimeline Timeline;

	class ACPlayer* Player;
	bool IsCameraPlaying = false;
};
