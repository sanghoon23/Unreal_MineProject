#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "CAIC_HM_Basic.generated.h"

UENUM(BlueprintType)
enum class AIBehaviorState : uint8
{
	NONE = 0,

};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_Basic 
	: public AAIController
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	//UPROPERTY(VisibleAnywhere, Category = "AIControl")
	//	class UAIPerceptionComponent* Perception;

	//UPROPERTY(VisibleAnywhere, Category = "AIControl")
	//	class UAISenseConfig_Sight* Sight;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float PatrolRadius = 600.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float DetectRadius = 900.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float HangAround = 500.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float AttackRange = 220.0f;

	//UPROPERTY(VisibleAnywhere, Category = "Behavior")
	//	class UBehaviorTree* BT;

	//UPROPERTY(VisibleAnywhere, Category = "Behavior")
	//	class UBlackboardData* BB;

	#pragma endregion

public:
	ACAIC_HM_Basic();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	//UFUNCTION()
	//	void OnSensingTarget(AActor* Actor, FAIStimulus Stimulus);

private:
	class UBehaviorTree* BT;
	class UBlackboardData* BB;
};
