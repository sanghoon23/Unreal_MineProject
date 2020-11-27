#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "CPL_CineCameraActor.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACPL_CineCameraActor 
	: public ACineCameraActor
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "Data")
		float TimeArrivalToTaget = 2.5f;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		float LocationSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		float RotationInterpSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Actor")
		AActor* Target;

public:
	UFUNCTION(BlueprintCallable)
		void MoveToTarget();

#pragma endregion

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	bool bStart = false;
};
