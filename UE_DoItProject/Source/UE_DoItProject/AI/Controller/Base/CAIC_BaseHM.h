#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIC_BaseHM.generated.h"

UCLASS()
class UE_DOITPROJECT_API ACAIC_BaseHM 
	: public AAIController
{
	GENERATED_BODY()
	
#pragma region Reflection
protected:
	UPROPERTY(EditAnywhere, Category = "AIControl")
		float PatrolRadius = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float DetectRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float AttackRange = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Data")
		TArray<int> PatternAttackTypeList;

	UPROPERTY(EditAnywhere, Category = "Data")
		TArray<int> RandomAttackTypeList;

#pragma endregion

public:
	ACAIC_BaseHM();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	/* Function */
public:
	int GetTypePatternAttackState();
	int GetTypeRandomAttackState();

	//@Type 으로 받으려고 virtual 선언 안함.
	//virtual void AddTypePatternAttack(EHM_AssassinAttackType Type) {}
	////TODO : EraseType

	//virtual void AddTypeRandomAttack(EHM_AssassinAttackType Type) {}

protected:
	class UBehaviorTree* BT;
	class UBlackboardData* BB;

};
