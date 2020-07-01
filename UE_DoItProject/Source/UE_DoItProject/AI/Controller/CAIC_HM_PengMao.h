#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIC_HM_PengMao.generated.h"

UENUM(BlueprintType)
enum class EAIState_PengMao : uint8
{
	DISCOVERY = 0, //@적발견
	TRYATTACK = 1, //@공격 시도
	FINDATTACKPOINT = 2, //@
};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_PengMao 
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
		float AttackRange = 220.0f;

	#pragma endregion

public:
	ACAIC_HM_PengMao();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	class UBehaviorTree* BT;
	class UBlackboardData* BB;
	
};
