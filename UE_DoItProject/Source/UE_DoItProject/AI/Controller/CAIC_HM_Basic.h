#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"

#include "CAIC_HM_Basic.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_Basic : public AAIController
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(VisibleAnywhere, Category = "AIControl")
		class UAIPerceptionComponent* Perception;

	UPROPERTY(VisibleAnywhere, Category = "AIControl")
		class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float PatrolRadius = 600.0f;

	#pragma endregion

public:
	ACAIC_HM_Basic();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
		void OnSensingTarget(AActor* Actor, FAIStimulus Stimulus);

private:
	class UBehaviorTree* BT;
	class UBlackboardData* BB;
};
