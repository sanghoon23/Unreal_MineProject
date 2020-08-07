#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIC_HM_Assassin.generated.h"

UENUM(BlueprintType)
enum class EAIState_Assassin : uint8
{
	DISCOVERY = 0, //@적 발견
	WAITATTACK = 1, //@공격 대기
	TRYATTACK = 2, //@공격 시도
	FINDATTACKPOINT = 3, //@
};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_Assassin 
	: public AAIController
{
	GENERATED_BODY()

#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "AIControl")
		float PatrolRadius = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float DetectRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float AttackRange = 500.0f;

#pragma endregion
	
public:
	ACAIC_HM_Assassin();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	class UBehaviorTree* BT;
	class UBlackboardData* BB;

};
