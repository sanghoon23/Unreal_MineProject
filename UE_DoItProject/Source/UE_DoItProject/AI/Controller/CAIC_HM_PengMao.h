#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIC_HM_PengMao.generated.h"

UENUM(BlueprintType)
enum class EAIState_PengMao : uint8
{
	NONE = 0, /* NONE - �ʱ�ȭ (AIState) */
	HANGAROUND = 1,
	CANATTACK = 2,
};

UCLASS()
class UE_DOITPROJECT_API ACAIC_HM_PengMao 
	: public AAIController
{
	GENERATED_BODY()

	#pragma region Reflection
private:
	UPROPERTY(EditAnywhere, Category = "AIControl")
		float PatrolRadius = 300.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float DetectRadius = 900.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float HangAround = 500.0f;

	UPROPERTY(EditAnywhere, Category = "AIControl")
		float AttackRange = 220.0f;

	UPROPERTY(VisibleAnywhere, Category = "AIControl")
		EAIState_PengMao InputFirstAIState = EAIState_PengMao::NONE;

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
