#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CPL_BlendCameraActor.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACPL_BlendCameraActor 
	: public ACameraActor
{
	GENERATED_BODY()
	
	#pragma	region Reflection
private:

	#pragma endregion

public:
	ACPL_BlendCameraActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
