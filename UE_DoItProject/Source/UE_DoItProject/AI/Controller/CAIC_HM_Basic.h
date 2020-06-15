#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "CAIC_HM_Basic.generated.h"

UENUM(BlueprintType)
enum class EAIState_Basic : uint8
{
	NONE		= 0, /* NONE - √ ±‚»≠ (AIState) */
	HANGAROUND	= 1,
	CANATTACK	= 2,
	FINDATTACKPOINT	= 3,
};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_Basic 
	: public AAIController
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "AIControl")
		float PatrolRadius = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float DetectRadius = 800.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float HangAround = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float AttackRange = 220.0f;

	UPROPERTY(VisibleAnywhere, Category = "AIControl")
		EAIState_Basic CurrentAIState = EAIState_Basic::NONE;

	UPROPERTY(VisibleAnywhere, Category = "Data")
		TArray<AActor*> IgnoreActors;

	#pragma endregion

public:
	ACAIC_HM_Basic();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	class UBehaviorTree* BT;
	class UBlackboardData* BB;
};
