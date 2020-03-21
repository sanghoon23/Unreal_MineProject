#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraSpline_FinalAttack.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACameraSpline_FinalAttack 
	: public AActor
{
	GENERATED_BODY()

	#pragma	region Reflection
private:

	#pragma endregion

public:	
	ACameraSpline_FinalAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	#pragma	region Member
private:

	#pragma endregion
};
